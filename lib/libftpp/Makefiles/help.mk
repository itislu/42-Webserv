ifndef HELP_MK
HELP_MK			:=	1

include				$(shell git rev-parse --show-toplevel)/Makefiles/setup.mk
include				$(MAKEFILES_DIR)/print.mk


# ****************************** HELP TARGETS ******************************** #

help			:
					echo -e "Targets:"
					echo -e "  all              Build the project (default target)."
					echo -e "  run              Build and run the project."
					echo -e "  opt              Rebuild the project with optimizations."
					echo -e "  san              Rebuild the project with sanitizers."
					echo -e "  val              Build and run the project with valgrind."
					echo -e "  valfd            Build and run the project with valgrind and file descriptor tracking."
					echo -e "  term             Build and run the project in a new terminal window."
					echo -e "  clear            Clear the terminal, build the project and clear again."
					echo -e "  re               Rebuild the project."
					echo -e "  libs             Build all libraries the project depends on."
					echo -e "  bear             Generate compilation database using Bear."
					echo -e "  doxygen          Generate documentation using Doxygen."
					echo -e "  uml              Generate UML diagrams using clang-uml and convert them to PNG, SVG and PDF."
					echo -e "  clean            Remove build artifacts."
					echo -e "  fclean           Remove build artifacts and executable."
					echo -e "  ffclean          Remove build artifacts, executable and generated documentation without checking for unknown files."
					echo -e "  print-%          Print the value of a Makefile variable (replace % with variable name)."
					echo -e "  help             Display this message."
					echo -e "  help-% | %-help  Display more information for a specific target (replace % with target name)."
					echo
					echo -e "Environment Variables:"
					echo -e "  MODE             Build mode to combine multiple targets."
					echo -e "  ARGS             If specified, the program will run with these arguments after compilation."
					echo -e "  TERMINAL         Terminal emulator to use for targets opening a new terminal window."
					echo
					echo -e "Usage: make [\\$(STY_UND)target\\$(STY_RES)] [...] [MODE=<\\$(STY_UND)mode\\$(STY_RES)>,[...]] [ARGS=\"'<\\$(STY_UND)arg\\$(STY_RES)>' [...]\"] [TERMINAL=<\\$(STY_UND)terminal\\$(STY_RES)>]"

help-all all-help:
					echo -e "Build the project."
					echo -e "This is the default target when no target is specified."

help-run run-help:
					echo -e "Build the project and run the executable."
					echo -e "Arguments to the program can be passed via the ARGS variable."
					echo
					echo -e "Usage: make run [ARGS=\"'<\\$(STY_UND)arg\\$(STY_RES)>' [...]\"]"

help-opt opt-help:
					echo -e "Rebuild the project with the following compiler optimization flags:"
					echo -e "  $(CXXFLAGS_OPT)"

help-san san-help:
					echo -e "Rebuild the project with the following sanitizer flags:"
					echo -e "  $(CXXFLAGS_SAN)"

help-val val-help:
					echo -e "Build the project and run the executable with valgrind."
					echo -e "Arguments to the program can be passed via the ARGS variable."
					echo
					echo -e "The following valgrind flags are used:"
					echo -e "$(VALGRINDFLAGS)" | tr ' ' '\n' | sed 's/^/  /'
					echo
					echo -e "Usage: make val [ARGS=\"'<\\$(STY_UND)arg\\$(STY_RES)>' [...]\"]"

help-valfd valfd-help:
					echo -e "Build the project and run the executable with valgrind and file descriptor tracking."
					echo -e "A new terminal window is opened to avoid inheriting open file descriptors."
					echo -e "Arguments to the program can be passed via the ARGS variable."
					echo
					echo -e "The following valgrind flags are used:"
					echo -e "$(VALGRINDFLAGS)" | tr ' ' '\n' | sed 's/^/  /'
					echo -e "File descriptor specific flags:"
					echo -e "$(VALGRINDFDFLAGS)" | tr ' ' '\n' | sed 's/^/  /'
					echo
					echo -e "Usage: make valfd [ARGS=\"'<\\$(STY_UND)arg\\$(STY_RES)>' [...]\"]"

help-term term-help:
					echo -e "Build the project and run the executable in a new terminal window."
					echo -e "The terminal emulator used is determined by the TERMINAL variable."
					echo -e "Arguments to the program can be passed via the ARGS variable."
					echo
					echo -e "The following terminal emulator is used by default:"
					echo -e "  $(TERMINAL)"
					echo
					echo -e "Usage: make term [TERMINAL=<\\$(STY_UND)terminal\\$(STY_RES)>] [ARGS=\"'<\\$(STY_UND)arg\\$(STY_RES)>' [...]\"]"

help-clear clear-help:
					echo -e "Clear the terminal, build the project and if successful, clear the terminal again."

help-re re-help	:
					echo -e "Rebuild the project."

help-libs libs-help:
					echo -e "Build all libraries the project depends on."
					echo
					echo -e "The following libraries will be built:"
					for lib in $(LIBS_LOCAL); do \
						echo -e "  $$lib"; \
					done

help-bear bear-help:
					echo -e "Generate compilation database (compile_commands.json) using Bear."
					echo -e "The compilation database is used by tools like clangd to significantly speed up workspace parsing."
					echo -e "It is also required for tools like doxygen or clang-uml that need compilation information to analyze the codebase."
					echo -e "The compilation database is generated by running 'bear -- make re'."

help-doxygen doxygen-help:
					echo -e "Generate documentation using Doxygen."
					echo -e "A Doxyfile is required to configure the documentation generation."
					echo -e "If no Doxyfile exists, a default configuration file will be created."
					echo -e "The documentation will be generated in $(DOXYGEN_OUTDIR)."
					echo -e "After generation, the HTML documentation will be opened in the default browser."

help-uml uml-help:
					echo -e "Generate UML diagrams using clang-uml and convert them to PNG, SVG and PDF."
					echo -e "A .clang-uml configuration file is required to configure the UML generation."
					echo -e "If no .clang-uml configuration file exists, a default configuration file will be created."
					echo -e "The generated files will be saved in $(UML_OUTDIR)."
					echo -e "After generation, the output directory will be opened."

help-clean clean-help:
					echo -e "Remove build artifacts."

help-fclean fclean-help:
					echo -e "Remove build artifacts and the executable."

help-ffclean ffclean-help:
					echo -e "Remove build artifacts, the executable and generated documentation without checking for unknown files."
					echo
					echo -e "The following directories will be removed:"
					echo -e "  $(OBJ_DIR)"
					echo -e "  $(DEP_DIR)"
					echo -e "  $(DOXYGEN_OUTDIR)"
					echo -e "  $(UML_OUTDIR)"

help-print print-help:
					echo -e "Print the value of a Makefile variable by appending the variable name to print-..."
					echo -e "Useful for Makefile debugging."
					echo
					echo -e "Usage: make print-<\\$(STY_UND)variable name\\$(STY_RES)>"

help-help		:
					echo -e "Display more information for a specific target by appending or prepending help."
					echo
					echo -e "Usage: make help-<\\$(STY_UND)target\\$(STY_RES)> | make <\\$(STY_UND)target\\$(STY_RES)>-help"

help-MODE MODE-help:
					echo -e "Build mode to combine with other targets."
					echo -e "Multiple modes can be combined by separating them with a space."
					echo
					echo -e "Usage: make <\\$(STY_UND)target\\$(STY_RES)> MODE=<\\$(STY_UND)mode\\$(STY_RES)>,[...]"

help-ARGS ARGS-help:
					echo -e "If specified, the program will run with these arguments after compilation."
					echo -e "The string from the ARGS variable undergoes shell expansion."
					echo -e "To pass special characters like $$, *, ~, etc. literally, they must be properly escaped."
					echo
					echo -e "Usage: make <\\$(STY_UND)target\\$(STY_RES)> ARGS=\"'<\\$(STY_UND)arg\\$(STY_RES)>' [...]\""

help-TERMINAL TERMINAL-help:
					echo -e "Override the default terminal emulator for targets opening a new terminal window."
					echo
					echo -e "Usage: make <\\$(STY_UND)target\\$(STY_RES)> TERMINAL=<\\$(STY_UND)terminal\\$(STY_RES)>"


endif
