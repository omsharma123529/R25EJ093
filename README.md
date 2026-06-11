2D Graphics Editor in C
Course: Advanced C Programming using Gen AI
Level: First Year B.Tech
File: graphics_editor.c

What is this?
A terminal-based 2D graphics editor written entirely in C. It lets you draw shapes on a character canvas made up of only two characters — * for drawn pixels and _ for empty space. You can add shapes, delete them, modify their parameters, and print the canvas to the terminal at any time.

Features
Four drawable shapes — Circle, Rectangle, Line, Triangle
Full object management — Add, Delete, Modify, and List objects
Character canvas — 40 rows × 80 columns, using only * and _
Numbered ruler — Row and column indices printed alongside the canvas for easy coordinate reference
Persistent object store — Up to 50 objects stored simultaneously
Input validation — Bad values (e.g. negative radius, inverted rectangle) are caught and rejected gracefully
Canvas Layout
    0         10        20        30   ...
    |---------|---------|---------|---------
  0|__________________________________________________________________|
  1|__________________________________________________________________|
 ...
 39|__________________________________________________________________|
    ------------------------------------------------------------------
x = column (0 – 79, left to right)
y = row (0 – 39, top to bottom)
Every cell is either * (part of a shape) or _ (background)
How to Compile
You need GCC and the standard math library. Run this one command in your terminal:

gcc -Wall -o graphics_editor graphics_editor.c -lm
Flag	Purpose
-Wall	Enable all compiler warnings
-o graphics_editor	Name the output executable
-lm	Link the math library (required for math.h)
How to Run
./graphics_editor
You will see the main menu:

╔══════════════════════════════════╗
║     2D GRAPHICS EDITOR           ║
╠══════════════════════════════════╣
║  1. Display canvas               ║
║  2. Add object                   ║
║  3. Delete object                ║
║  4. Modify object                ║
║  5. List objects                 ║
║  6. Clear all objects            ║
║  0. Exit                         ║
╚══════════════════════════════════╝
Type a number and press Enter to choose an option.

Shape Reference
1. Circle
Drawn using the Midpoint Circle Algorithm (integer-only, no floating point).

Input	Description
Center x	Column of the centre (0–79)
Center y	Row of the centre (0–39)
Radius	Must be greater than 0
Example — center (40, 20), radius 10:

Add → Circle → x: 40 → y: 20 → radius: 10
2. Rectangle
Drawn as four connected lines (top, bottom, left, right edges only — not filled).

Input	Description
Top-left x, y	Upper-left corner coordinate
Bottom-right x, y	Lower-right corner (must be strictly greater than top-left)
Example — from (5, 5) to (30, 15):

Add → Rectangle → x1: 5 → y1: 5 → x2: 30 → y2: 15
3. Line
Drawn using Bresenham's Line Algorithm — works at any angle, any direction.

Input	Description
Start x, y	Starting point
End x, y	Ending point
Example — diagonal from (0, 0) to (79, 39):

Add → Line → x1: 0 → y1: 0 → x2: 79 → y2: 39
4. Triangle
Drawn as three lines connecting three vertices (not filled).

Input	Description
Vertex 1 x, y	First corner
Vertex 2 x, y	Second corner
Vertex 3 x, y	Third corner
Example — triangle with vertices at (40,2), (10,35), (70,35):

Add → Triangle → x1:40 y1:2 → x2:10 y2:35 → x3:70 y3:35
Menu Options in Detail
Option 2 — Add Object
Prompts you to choose a shape type, then asks for the relevant coordinates. The shape is drawn immediately and assigned an ID.

Option 3 — Delete Object
Lists all active objects with their IDs and parameters. Enter the ID of the object you want to remove. Deleted objects are flagged internally; the canvas is rebuilt without them.

Option 4 — Modify Object
Lists all active objects, asks you to pick one by ID, then prompts for new values. The canvas is redrawn with the updated parameters.

Option 5 — List Objects
Prints a summary of all active objects and their current parameters without touching the canvas.

Option 6 — Clear All
Removes every object and resets the canvas to all _.

Option 0 — Exit
Exits the program cleanly.

Code Structure
graphics_editor.c
│
├── Section 1 – Canvas helpers
│   ├── clearCanvas()      — fill entire canvas with '_'
│   ├── plotPoint(r, c)    — set canvas[r][c] = '*'
│   └── erasePoint(r, c)   — set canvas[r][c] = '_'
│
├── Section 2 – Drawing primitives
│   ├── drawLine()         — Bresenham's line algorithm
│   ├── drawCircle()       — Midpoint circle algorithm
│   ├── drawRectangle()    — four calls to drawLine()
│   └── drawTriangle()     — three calls to drawLine()
│
├── Section 3 – Canvas rebuild
│   └── rebuildCanvas()    — clear + replay all active objects
│
├── Section 4 – Display
│   └── displayCanvas()    — print canvas with row/col ruler
│
├── Section 5 – Object listing
│   └── listObjects()      — print all active object parameters
│
├── Section 6 – Add
│   └── addObject()        — prompt user, store in objects[]
│
├── Section 7 – Delete
│   └── deleteObject()     — set active = 0, rebuild
│
├── Section 8 – Modify
│   └── modifyObject()     — update stored params, rebuild
│
└── Section 9 – Entry point
    ├── printMenu()        — display the menu
    └── main()             — do-while loop driving the program
Key Design Decisions
Why rebuildCanvas() instead of erasing individual pixels?
Shapes can overlap. If you delete one overlapping shape and try to erase only its pixels, you'd accidentally erase part of another shape too. Clearing the whole canvas and replaying all active objects avoids this problem entirely.

Why an active flag instead of shifting the array?
Shifting objects in the array would change every object's ID after a deletion. Using a flag keeps IDs stable and makes delete an O(1) operation.

Why Bresenham's and Midpoint algorithms?
Both use only integer arithmetic — no sin, cos, or sqrt needed. This means no floating-point rounding errors and faster execution.

Limits & Constraints
Item	Limit
Canvas width	80 columns
Canvas height	40 rows
Max objects	50
Canvas characters	* and _ only
Coordinate system	x = col (0–79), y = row (0–39)
Coordinates outside the canvas bounds are silently ignored by plotPoint(), so shapes that partially go off-screen will not crash the program.

Example Session
Choice: 2          ← Add object
Shape: 1           ← Circle
Center x: 40
Center y: 20
Radius: 10
Object added successfully (ID 1).

Choice: 2          ← Add another
Shape: 3           ← Line
Start x: 0   Start y: 0
End   x: 79  End   y: 39
Object added successfully (ID 2).

Choice: 1          ← Display canvas
(canvas printed with circle + diagonal line)

Choice: 3          ← Delete
Enter ID: 1
Object 1 deleted.  ← Circle gone, line remains

Choice: 0          ← Exit
Goodbye!
Requirements
GCC compiler (any version supporting C89/C99)
Standard C libraries: stdio.h, stdlib.h, string.h, math.h
A terminal wide enough to display 80+ characters per line
