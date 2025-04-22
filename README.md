# sweep-them-mines
A nod to the classic MineSweeper from Windows 98

![image](https://github.com/user-attachments/assets/9d879246-8a16-4741-a11a-3910f5c43adb)

# General Notes:

- The `Images` sub folder is required for the game to function. Custom images can be used.
- The rules are similar to that of the Win98 version of MineSweeper.
- When running the game, this folder is created:
  ```
  %localappdata%\ASWSoftware\SweepThemMines
  ```
  where there will be a `SweeThemMines.ini`. Within this settings file, a custom images folder can be specified using property `ImagesPath`.
  Note: Logging settings are there as well, however, logging is not yet implemented, as of this writing.

# Donations:

Pizza money donations are always appreciated:

PayPal:
donate@aswsoftware.com

BitCoin:
15rKqL1numHJyE36ottMbhs5cmCjJkuowV

# Coding Standards

To use uncrustify (for coding standards (pretty formatting) for this repo):

1. Get `uncrustify` and ensure that the path to `uncrustify.exe` is added to the Windows `Path` environment variable.
    https://github.com/uncrustify/uncrustify

2. Run the following command locally to set the git hooks directory (uncrustify will run upon commit):
```
git config --local core.hooksPath .githooks/
```
