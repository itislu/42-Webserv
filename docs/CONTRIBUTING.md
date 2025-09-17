# Table of Contents

- [Commit Messages](#commit-messages)
  - [Commit types](#commit-types)
  - [Commit scope (optional)](#commit-scope-optional)
  - [Commit description](#commit-description)
  - [Commit body (optional)](#commit-body-optional)
  - [Commit footer (optional)](#commit-footer-optional)
- [Branches](#branches)
- [Create a Pull Request](#create-a-pull-request)
  - [Git part](#git-part)
  - [GitHub part](#github-part)
  - [Dependent pull request](#dependent-pull-request)
- [Review a Pull Request](#review-a-pull-request)
  - [Suggestion branch](#suggestion-branch)
- [Git Cheatsheet](#git-cheatsheet)
  - [Commit parts of files](#commit-parts-of-files)
  - [Change the last commit](#change-the-last-commit)
  - [Change commit history](#change-commit-history)

# Commit Messages

We follow the [Conventional Commits](https://www.conventionalcommits.org/en/) specification.

```txt
<type>[(scope)]: <description>
```

## Commit types

| Type      | Description                                                                            | Example                                                             |
|-----------|----------------------------------------------------------------------------------------|---------------------------------------------------------------------|
| `feat`    | A new feature for the user or the codebase.                                            | `feat: Add logging for HTTP requests`                               |
| `improve` | An enhancement to an existing feature. Use for changes too minor for `feat`.           | `improve: Make error messages more descriptive`                     |
| `fix`     | Fix of a bug or potential bug.                                                         | `fix: Handle URI decoding correctly`                                |
| `refactor`| Structural code change that doesn't change program behavior.                           | `refactor: Restructure config parser to be easier to extend`        |
| `style`   | Code style, not style of the program output. Significant logic changes are `refactor`. | `style: Rename local variables`                                     |
| `perf`    | Change made purely for measurable performance improvements.                            | `perf: Use new hash function for 2x address lookup speed`           |
| `test`    | Addition or fix of tests.                                                              | `test: Add unit test for parsing HTTP header`                       |
| `docs`    | Addition or fix of documentation or comments only, in any file.                        | `docs: Add install guide to readme`                                 |
| `build`   | Related to build system and dependencies.                                              | `build: Use more compiler warnings for default build`               |
| `devops`  | Related to development automations and CI/CD.                                          | `devops: Add formatting check to GitHub Actions`                    |
| `chore`   | Repository maintenance / "housekeeping". Does not touch source files.                  | `chore: Whitelist image files in .gitignore`                        |
| `revert`  | Git reverts.                                                                           | `revert: "perf: Use new hash function for 2x address lookup speed"` |

## Commit scope (optional)

The scope is optional but provides helpful context. For source code changes it is often the name of the component or directory being modified.

The scope is placed in parentheses after the commit type.

- `feat(parser): Add support for nested locations`
- `fix(http): Handle chunked encoding correctly`
- `docs(readme): Update installation instructions`

> [!TIP]
> Whenever you find yourself writing the location of the change in the description, use a scope instead.

## Commit description

The description should be a concise summary of the change.

| Guideline                                  | Explanation                                                                                 |
|--------------------------------------------|---------------------------------------------------------------------------------------------|
| Use imperative mood.                       | Write what the commit *does*, not what it *did* (e.g., `Add feature`, not `Added feature`). |
| Start with a verb.                         | Prevents ambiguity.                                                                         |
| Capitalize the first letter.               | Improves readability in long commit histories.                                              |
| Do not end with a period.                  | Convention.                                                                                 |
| Keep the line length within 72 characters. | GitHub truncates titles at this length.                                                     |

Revert commits may have the message of the reverted commit in quotes instead:

- `revert: "perf: Use new hash function for 2x address lookup speed"`
- `revert: Revert cd1de75, 0d32423`

> [!NOTE]
>
> Always starting with a verb prevents ambiguity. For example, `fix: Session expiration` is unclear. Does it mean the session expired but shouldn't, or the opposite?
>
> Using a more specific verb clarifies the intent:
>
> - `fix: Prevent session expiration`
> - `fix: Ensure session expiration`
>
> While some cases are unambiguous even without a verb (`fix: Memory leak`), consistency is preferred.
>
> Adding a verb only to end up with a title like `fix: Fix memory leak` might seem redundant, but using a verb is still preferred for consistency and clarity.
> `Fix` as a verb usually can and should be replaced with a more specific one anyway (see `fix: Fix session expiration`).

## Commit body (optional)

The body should describe the **rationale** behind the changes (the *why*, not just the *what*).

The body must be separated from the header by a **blank line**.

## Commit footer (optional)

If a commit resolves an issue on GitHub, the issue number should be mentioned at the end of the commit message on a separate line, prefixed with `Closes`:

- `Closes #234`
- `Closes #123, #245, #992`

This links commits to issues automatically and closes them if they land on the `main` branch.

# Branches

```txt
<type>/<description>
```

**`type`**:

- Same [types](#commit-messages) as commit messages.

**`description`**:

- kebab-case (lowercase, words separated by hyphens).
- Words may only contain alphanumeric characters or dots.

**Examples:**

- `feat/add-login-page`
- `fix/uri-decoding`

# Create a Pull Request

## Git part

1. Update local `main` branch:

   ```txt
   git switch main &&
   git pull
   ```

2. Switch to the branch you want to make the PR with:

   ```txt
   git switch <BRANCH_NAME>
   ```

3. Update your branch with `main`:

   ```txt
   git merge main
   ```

   - If a conflict occurs, resolve it, then:

     ```txt
     git add -p &&
     git merge --continue
     ```

4. Push your branch to GitHub.

   ```txt
   git push -u origin HEAD
   ```

> [!TIP]
> You can update the local `main` branch without switching to it:
>
> ```txt
> git fetch origin main:main
> ```

## GitHub part

1. **Create a pull request on GitHub:**

   `Pull requests` tab -> `New pull request` -> Select target (`base`) and source (`compare`) branches.

2. **Title and description:**

   **Title format**: Always start with an uppercase imperative verb.

3. **Assign labels and a milestone.**

4. **Normal vs draft pull request:**

   - **Normal PR**: You are sure your branch is ready to be merged.
   - **Draft PR**: You want to finish the description later or still need to push a few more commits.

5. **Request reviewer(s) (optional):**

   - If you want to choose explicitly who should review your PR, request a reviewer.
   - You can request multiple reviewers if you think more pairs of eyes should take a look.

## Dependent pull request

If you need to start work that depends on changes from a branch that is still under review (not yet merged):

1. **Branch off the in-review branch.**

   Create your new branch from the branch that is still under review, not from `main`:

   ```txt
   git switch feat/login-form &&
   git pull &&
   git switch -c feat/oauth-support
   ```

2. **Mark your pull request as a draft.**

   When you open a PR for your new branch, mark it as a **draft** until the base branch has been merged.

3. **Mention the dependency in the PR description.**

   In your PR description, clearly state that your branch depends on another PR and link to it:
   > `* Depends on: #123`

   This helps reviewers understand the relationship between PRs.

  > [!TIP]
  > The `*` is useful to enable GitHub's title preview. With the preview you can see at one glance if the base branch has been merged already or not.

4. **Once the base branch has been merged:**

   Update your branch to include the latest changes from `main` and mark your PR as ready for review.

> [!NOTE]
> While your PR is open, you will see extra commits from the base branch in the commit list. They will disappear automatically once you update your branch after the base branch has been merged.

# Review a Pull Request

1. **Go to the "Files changed" tab.**

2. **Leave feedback:**

   - **Comment**: Use this for a single, standalone comment. It's sent immediately.
   - **Start a review**: This is the preferred method. It allows you to batch multiple comments together into a single review. Your comments are held as "pending" until you submit the full review, which prevents spamming the author with notifications.

3. **Submit review:**

   - **Approve**: The changes are good to go.
   - **Request changes**: The PR needs modifications before it can be merged.
   - **Comment**: You have general feedback but are not formally approving or requesting changes.

4. **Merge.**

   If all requested reviewers approved and there are no more requested changes, you can merge the PR.

> [!TIP]
> You can click-and-drag or shift-click on the line numbers to select a range of lines your comment applies to.

> [!TIP]
> To suggest specific code changes, use a `suggestion` block in your comment.
> The author can then apply your suggestion with a single click.
>
> ````md
> ```suggestion
> // Suggested code replacing the selected lines.
> ```
> ````

> [!NOTE]
> **About assignments**
>
> Assignments on PRs are commonly used to take ownership of the PR, ensuring it progresses and gets merged.
> In a small team this is usually not needed.
>
> Instead, all reviewers of a PR can automatically get assigned to it, making it easy to see in the list of PRs who is involved.

## Suggestion branch

If you as a reviewer want to suggest a larger change to a PR and know how to implement it, you can create a new branch from the PR's source branch and create a new PR with that original source branch as your target branch.

The name of the suggestion branch should follow the usual branch naming guidelines.

![PR suggestion branch](/docs/assets/pr-suggestion-branch.png)

# Git Cheatsheet

## Commit parts of files

If you want to stage only specific changes from your working directory (only some files or just parts of a file), use:

```txt
git add -p
```

`-p` stands for "patch" mode. This command lets you interactively review each change ("hunk") and decide whether to stage it.
You can split hunks, skip them, or stage only the parts you want. Use `?` to see what each letter does.

This is useful for making clean, focused commits.

## Change the last commit

If you made a mistake in your last commit message or forgot to include a file, you can fix it with:

```txt
git commit --amend
```

If you don't want to change the commit message:

```txt
git commit --amend --no-edit
```

## Change commit history

With interactive rebase you can fix old commit messages, squash small fixup commits, and even reorder commits.

To edit any of the last 10 commits of the current branch:

```txt
git rebase -i HEAD~10
```

If you do this on a branch that already has an open PR, you should preserve commit dates to keep the pull request page on GitHub in chronological order:

```txt
git rebase -i --committer-date-is-author-date HEAD~10
```

> [!NOTE]
> You need Git version 2.29 to use `--committer-date-is-author-date` in combination with `-i`.

Because you are still changing the commit hashes, you will need to force push:

```txt
git push --force-with-lease
```

`--force-with-lease` is generally preferred over `--force` because it protects against accidentally overwriting changes on the remote branch that you do not have locally.
