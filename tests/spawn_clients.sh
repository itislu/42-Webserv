#!/bin/bash
# spawn_clients_nc.sh
# Requires: nc (netcat)
# Keeps connections open by piping an infinite stream into nc (tail -f /dev/null)

HOST="127.0.0.1"
PORTS=(8080 8081)
CLIENTS_PER_PORT=1000       # number of simultaneous clients per port
INITIAL_MSG="Hello from client"  # empty to skip
RUN_SECONDS=0             # 0 = run until you Ctrl+C or kill PIDs

pids=()

if ! command -v nc >/dev/null 2>&1; then
  echo "Error: nc (netcat) not found. Use the bash /dev/tcp version instead."
  exit 1
fi

for port in "${PORTS[@]}"; do
  for ((i=0;i<CLIENTS_PER_PORT;i++)); do
    if [ -n "$INITIAL_MSG" ]; then
      # send initial message then keep the connection open
      ( printf "%s\n" "$INITIAL_MSG (port ${port})"; tail -f /dev/null ) | nc "$HOST" "$port" &
    else
      ( tail -f /dev/null ) | nc "$HOST" "$port" &
    fi
    pids+=( $! )
    # sleep 0.05
  done
done

echo "Spawned ${#pids[@]} clients. PIDs:"
printf '%s\n' "${pids[@]}"

if [ "$RUN_SECONDS" -gt 0 ]; then
  echo "Running for $RUN_SECONDS seconds..."
  sleep "$RUN_SECONDS"
  echo "Killing clients..."
  kill "${pids[@]}" 2>/dev/null
fi

echo "Done. To kill clients later: kill ${pids[*]}"
