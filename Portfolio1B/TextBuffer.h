
#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include "Stack.h"
#include <string>
#include <iostream>
#include <algorithm>

// Records one edit so it can be reversed later ( for UNDO/REDO)
//isTypeAction tells us whether this record was a TYPE or a DELETE
struct UndoRecord {
     bool isTypeAction;  // action type : true = TYPE , false = DELETE
     std::string text;
};


class TextBuffer {
private:
     std::string document;
     Stack<UndoRecord> undoStack;
     Stack<UndoRecord> redoStack;
     int undoCount;	// how many undo operations have been performed
     int redoCount;	// how many redo operations have been performed

public:
     TextBuffer() : undoCount(0), redoCount(0) {}

	// Appends 'text' to the document and records it so it can be undone later.
	// Doing this ( or any action) clears the redo stack, same as a real editor. 
     void typeText(const std::string& text)  {
	document += text;
	UndoRecord record;
	record.isTypeAction = true;
	record.text = text;
	undoStack.push(record);
	redoStack = Stack<UndoRecord>();
     }
	// Removes the last 'n' characters from the document(Clamped so it can't delete more than exists) and records what was removed.
     void deleteChars(int n) {
	if (n <= 0 || document.empty()) {
	     return;
	}
	int actualN = std::min(n, static_cast<int>(document.size()));
	std::string removed = document.substr(document.size() - actualN);
	document.erase(document.size() - actualN);

	UndoRecord record;
	record.isTypeAction = false;
	record.text = removed;
	undoStack.push(record);
	redoStack = Stack<UndoRecord>();

     }
     // Reverses the most recent action.IF it was a TYPE, we striped the type text back off. If it was a DELETE, WE put the removed text back. The record then goes onto the redostack so REDO can reapply it.
     void undo()  {
	if (undoStack.isEmpty())  {
	     std::cout << "Nothing to undo." << std::endl;
	     return;
	}
	UndoRecord record = undoStack.pop();

	if (record.isTypeAction)  {
	    document.erase(document.size() - record.text.size());
	} else  {
	     document += record.text;
	}
	redoStack.push(record);
	undoCount++;
     }

    //Reapplies the most recently undone action, doing the opposite of undo().
    // The record  then goes back onto the undo stack so it can be undone again.
     void redo()  {
	if (redoStack.isEmpty())  {
	     std::cout << "Nothing to redo." << std::endl;
	     return;
	}
	UndoRecord record = redoStack.pop();

	if (record.isTypeAction) {
	     document += record.text;
	} else {
	     document.erase(document.size() - record.text.size());
	}

	undoStack.push(record);
	redoCount++;
      }

     // prints the current document
      void print() const  {
	     std::cout << "Document: \"" << document << "\"" << std::endl;
      }

      int getUndoCount() const { return undoCount; }
      int getRedoCount() const { return redoCount; }
      const std::string& getFinalDocument() const { return document; }
};

#endif
