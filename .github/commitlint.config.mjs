export default {
  /*
   * Resolve and load @commitlint/config-conventional from node_modules.
   * Referenced packages must be installed
   */
  extends: ["@commitlint/config-conventional"],
  /*
   * Any rules defined here will override rules from @commitlint/config-conventional
   *
   * Still passes but should not:
   * - fix(): Description
   * - fix(scope ): Description
   * - fix(scope): DESCRIPTION
   */
  rules: {
    "body-leading-blank": [2, "always"],
	  "body-max-line-length": [2, "always", Infinity],
    "header-max-length": [2, "always", 72],
    "scope-case": [
      2,
      "always",
      [
        "lower-case",
        "upper-case",
        "camel-case",
        "kebab-case",
        "pascal-case",
        "snake-case"
      ]
    ],
    "scope-min-length": [2, "always", 1],
    "subject-case": [2, "always", ["sentence-case"]],
    "type-enum": [
      2,
      "always",
      [
        "feat",
        "improve",
        "fix",
        "refactor",
        "style",
        "perf",
        "test",
        "docs",
        "build",
        "devops",
        "chore",
        "revert",
      ],
    ],
  },
  /*
   * Array of functions that return true if commitlint should ignore the given message.
   * Given array is merged with predefined functions, which consist of matchers like:
   *
   * - 'Merge pull request', 'Merge X into Y' or 'Merge branch X'
   * - 'Revert X'
   * - 'v1.2.3' (ie semver matcher)
   * - 'Automatic merge X' or 'Auto-merged X into Y'
   *
   * To see full list, check https://github.com/conventional-changelog/commitlint/blob/master/%40commitlint/is-ignored/src/defaults.ts.
   * To disable those ignores and run rules always, set `defaultIgnores: false` as shown below.
   */
//   ignores: [(commit) => commit === ""],
  /*
   * Whether commitlint uses the default ignore rules, see the description above.
   */
  defaultIgnores: true,
  /*
   * Custom URL to show upon failure
  */
 helpUrl:
  "https://github.com/conventional-changelog/commitlint/#what-is-commitlint\n\
               https://github.com/itislu/42-Webserv?tab=contributing-ov-file#commit-messages",
};
