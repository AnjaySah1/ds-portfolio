Reflection - Portfolio 1B (Undo/Redo Text Buffer)

I built the Stack<T> class first,since TextBuffer depends on it, then designed the UndoRecord struct, then wrote TextBuffer and finished with the command loop that reads TYPE/DELTE/UNDO/REDO/PRINT.

The hardest part was getting Stack's operator= right in this part 1b. While typing it into nano on the server, I accidently left the function incomplete: it only had the self assignment check and never actually called clear() and copyFrom() or closed the function.Because the closing brace was missing,every function I wrote after it(push,pop,top) ended up nested inside operator=
instead of being separate functions, which the complier flagged in a confusing way since it thought my test file's main() was a member of Stack . I also had a bug in undo() where I accidentally type { else { instead of  } else { , which broke the if/else structure in a way that was easy to miss just by looking at it.

I found both by compiling often and reading the error messages closely instead of assuming the first error listed was the acutal problem; the brace mismatch especially cause a long chain of unrelated looking errors that all traced back to one missing }. Once I fixed the structure, I retested by typing a sequence of TYPE, UNDO, REDO, and DELETE commands by hand and checking the printed document matched what I expected at each step.

If I started over, I would double check brace matching right after writing a function instead of after writing several in a row, since one missing brace cascades into a lot of confusing errors further down. I understand now why undo/redo pairs well with a stack: reversing the most recent action and being able to rapply it both naturally fit push/pop order, which is part of why real editors are built this way.
