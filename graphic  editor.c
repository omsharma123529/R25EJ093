/*
 * ============================================================
 *  2D Graphics Editor — Advanced C Programming (Gen AI Class)
 *  First Year B.Tech Student Project
 * ============================================================
 *  Canvas  : 40 rows x 80 columns of '*' and '_'
 *  Objects : Circle, Rectangle, Line, Triangle
 *  Actions : Add, Delete, Modify, Display
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ─── Canvas dimensions ─────────────────────────────────── */
#define ROWS        40
#define COLS        80
#define MAX_OBJECTS 50

/* ─── Shape type identifiers ────────────────────────────── */
#define SHAPE_CIRCLE    1
#define SHAPE_RECTANGLE 2
#define SHAPE_LINE      3
#define SHAPE_TRIANGLE  4

/* ─── Canvas (only '*' and '_' are ever stored here) ──────── */
char canvas[ROWS][COLS];

/* ─── Object data structures ─────────────────────────────── */
typedef struct {
    int type;       /* SHAPE_* constant          */
    int active;     /* 1 = exists, 0 = deleted   */

    /* Circle  : cx, cy, radius                  */
    /* Rect    : x1, y1 (top-left) x2, y2 (btm) */
    /* Line    : x1, y1 -> x2, y2               */
    /* Triangle: three vertices (x1y1)(x2y2)(x3y3) */
    int x1, y1, x2, y2, x3, y3;
    int radius;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

/* ═══════════════════════════════════════════════════════════
 *  SECTION 1 – Canvas helpers
 * ═══════════════════════════════════════════════════════════ */

/* Fill every cell with '_' (background) */
void clearCanvas(void) {
    int r, c;
    for (r = 0; r < ROWS; r++)
        for (c = 0; c < COLS; c++)
            canvas[r][c] = '_';
}

/* Plot a single '*' if (r,c) is within bounds */
void plotPoint(int r, int c) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = '*';
}

/* Erase a single '*' → '_' if within bounds */
void erasePoint(int r, int c) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = '_';
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 2 – Primitive drawing functions
 *              (all draw '*' onto the canvas)
 * ═══════════════════════════════════════════════════════════ */

/* Bresenham's line algorithm */
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        plotPoint(y1, x1);          /* canvas is [row][col] = [y][x] */
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

/* Midpoint circle algorithm */
void drawCircle(int cx, int cy, int radius) {
    if (radius <= 0) return;
    int x = 0, y = radius;
    int d = 1 - radius;

    while (x <= y) {
        plotPoint(cy + y, cx + x);
        plotPoint(cy - y, cx + x);
        plotPoint(cy + y, cx - x);
        plotPoint(cy - y, cx - x);
        plotPoint(cy + x, cx + y);
        plotPoint(cy - x, cx + y);
        plotPoint(cy + x, cx - y);
        plotPoint(cy - x, cx - y);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

/* Rectangle using four lines */
void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1); /* top    */
    drawLine(x1, y2, x2, y2); /* bottom */
    drawLine(x1, y1, x1, y2); /* left   */
    drawLine(x2, y1, x2, y2); /* right  */
}

/* Triangle using three lines */
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 3 – Redraw everything from the objects[] list
 * ═══════════════════════════════════════════════════════════ */

void rebuildCanvas(void) {
    int i;
    clearCanvas();
    for (i = 0; i < objectCount; i++) {
        if (!objects[i].active) continue;
        switch (objects[i].type) {
            case SHAPE_CIRCLE:
                drawCircle(objects[i].x1, objects[i].y1, objects[i].radius);
                break;
            case SHAPE_RECTANGLE:
                drawRectangle(objects[i].x1, objects[i].y1,
                              objects[i].x2, objects[i].y2);
                break;
            case SHAPE_LINE:
                drawLine(objects[i].x1, objects[i].y1,
                         objects[i].x2, objects[i].y2);
                break;
            case SHAPE_TRIANGLE:
                drawTriangle(objects[i].x1, objects[i].y1,
                             objects[i].x2, objects[i].y2,
                             objects[i].x3, objects[i].y3);
                break;
            default:
                break;
        }
    }
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 4 – Display the canvas
 * ═══════════════════════════════════════════════════════════ */

void displayCanvas(void) {
    int r, c;
    printf("\n");
    /* Column ruler (every 10 cols) */
    printf("    ");
    for (c = 0; c < COLS; c += 10)
        printf("%-10d", c);
    printf("\n    ");
    for (c = 0; c < COLS; c++)
        printf("%c", (c % 10 == 0) ? '|' : '-');
    printf("\n");

    for (r = 0; r < ROWS; r++) {
        printf("%3d|", r);
        for (c = 0; c < COLS; c++)
            printf("%c", canvas[r][c]);
        printf("|\n");
    }
    printf("    ");
    for (c = 0; c < COLS; c++) printf("-");
    printf("\n");
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 5 – List active objects
 * ═══════════════════════════════════════════════════════════ */

void listObjects(void) {
    int i, found = 0;
    const char *names[] = { "", "Circle", "Rectangle", "Line", "Triangle" };

    printf("\n--- Active Objects ---\n");
    for (i = 0; i < objectCount; i++) {
        if (!objects[i].active) continue;
        found = 1;
        printf("  [%d] %s  ", i + 1, names[objects[i].type]);
        switch (objects[i].type) {
            case SHAPE_CIRCLE:
                printf("center=(%d,%d)  radius=%d",
                       objects[i].x1, objects[i].y1, objects[i].radius);
                break;
            case SHAPE_RECTANGLE:
                printf("top-left=(%d,%d)  bottom-right=(%d,%d)",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2);
                break;
            case SHAPE_LINE:
                printf("from=(%d,%d)  to=(%d,%d)",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2);
                break;
            case SHAPE_TRIANGLE:
                printf("v1=(%d,%d)  v2=(%d,%d)  v3=(%d,%d)",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2,
                       objects[i].x3, objects[i].y3);
                break;
        }
        printf("\n");
    }
    if (!found) printf("  (none)\n");
    printf("----------------------\n");
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 6 – Add objects
 * ═══════════════════════════════════════════════════════════ */

void addObject(void) {
    int choice;
    if (objectCount >= MAX_OBJECTS) {
        printf("Canvas is full (%d objects max).\n", MAX_OBJECTS);
        return;
    }

    printf("\n  Shape to add:\n");
    printf("  1. Circle\n  2. Rectangle\n  3. Line\n  4. Triangle\n");
    printf("  Choice: ");
    if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); return; }

    Object *o = &objects[objectCount];
    memset(o, 0, sizeof(Object));
    o->active = 1;

    switch (choice) {
        case 1:
            o->type = SHAPE_CIRCLE;
            printf("  Center x (col 0-%d): ", COLS - 1);
            scanf("%d", &o->x1);
            printf("  Center y (row 0-%d): ", ROWS - 1);
            scanf("%d", &o->y1);
            printf("  Radius: ");
            scanf("%d", &o->radius);
            if (o->radius <= 0) { printf("Radius must be > 0.\n"); return; }
            break;
        case 2:
            o->type = SHAPE_RECTANGLE;
            printf("  Top-left  x: "); scanf("%d", &o->x1);
            printf("  Top-left  y: "); scanf("%d", &o->y1);
            printf("  Bot-right x: "); scanf("%d", &o->x2);
            printf("  Bot-right y: "); scanf("%d", &o->y2);
            if (o->x2 <= o->x1 || o->y2 <= o->y1) {
                printf("Invalid rectangle (bottom-right must be > top-left).\n");
                return;
            }
            break;
        case 3:
            o->type = SHAPE_LINE;
            printf("  Start x: "); scanf("%d", &o->x1);
            printf("  Start y: "); scanf("%d", &o->y1);
            printf("  End   x: "); scanf("%d", &o->x2);
            printf("  End   y: "); scanf("%d", &o->y2);
            break;
        case 4:
            o->type = SHAPE_TRIANGLE;
            printf("  Vertex 1 x: "); scanf("%d", &o->x1);
            printf("  Vertex 1 y: "); scanf("%d", &o->y1);
            printf("  Vertex 2 x: "); scanf("%d", &o->x2);
            printf("  Vertex 2 y: "); scanf("%d", &o->y2);
            printf("  Vertex 3 x: "); scanf("%d", &o->x3);
            printf("  Vertex 3 y: "); scanf("%d", &o->y3);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    objectCount++;
    rebuildCanvas();
    printf("  Object added successfully (ID %d).\n", objectCount);
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 7 – Delete object
 * ═══════════════════════════════════════════════════════════ */

void deleteObject(void) {
    int id;
    listObjects();
    if (objectCount == 0) return;

    printf("  Enter object ID to delete (1-%d): ", objectCount);
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }

    if (id < 1 || id > objectCount || !objects[id - 1].active) {
        printf("  Invalid or already-deleted ID.\n");
        return;
    }
    objects[id - 1].active = 0;
    rebuildCanvas();
    printf("  Object %d deleted.\n", id);
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 8 – Modify object
 * ═══════════════════════════════════════════════════════════ */

void modifyObject(void) {
    int id;
    listObjects();
    if (objectCount == 0) return;

    printf("  Enter object ID to modify (1-%d): ", objectCount);
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }

    if (id < 1 || id > objectCount || !objects[id - 1].active) {
        printf("  Invalid or deleted ID.\n");
        return;
    }

    Object *o = &objects[id - 1];

    printf("  Modifying Object %d (%s) — enter new values:\n", id,
           o->type == SHAPE_CIRCLE    ? "Circle"    :
           o->type == SHAPE_RECTANGLE ? "Rectangle" :
           o->type == SHAPE_LINE      ? "Line"      : "Triangle");

    switch (o->type) {
        case SHAPE_CIRCLE:
            printf("  New center x: "); scanf("%d", &o->x1);
            printf("  New center y: "); scanf("%d", &o->y1);
            printf("  New radius  : "); scanf("%d", &o->radius);
            if (o->radius <= 0) { printf("Radius must be > 0.\n"); o->radius = 1; }
            break;
        case SHAPE_RECTANGLE:
            printf("  New top-left  x: "); scanf("%d", &o->x1);
            printf("  New top-left  y: "); scanf("%d", &o->y1);
            printf("  New bot-right x: "); scanf("%d", &o->x2);
            printf("  New bot-right y: "); scanf("%d", &o->y2);
            if (o->x2 <= o->x1 || o->y2 <= o->y1) {
                printf("Invalid rectangle. Keeping old values.\n");
                return;
            }
            break;
        case SHAPE_LINE:
            printf("  New start x: "); scanf("%d", &o->x1);
            printf("  New start y: "); scanf("%d", &o->y1);
            printf("  New end   x: "); scanf("%d", &o->x2);
            printf("  New end   y: "); scanf("%d", &o->y2);
            break;
        case SHAPE_TRIANGLE:
            printf("  New vertex 1 x: "); scanf("%d", &o->x1);
            printf("  New vertex 1 y: "); scanf("%d", &o->y1);
            printf("  New vertex 2 x: "); scanf("%d", &o->x2);
            printf("  New vertex 2 y: "); scanf("%d", &o->y2);
            printf("  New vertex 3 x: "); scanf("%d", &o->x3);
            printf("  New vertex 3 y: "); scanf("%d", &o->y3);
            break;
    }

    rebuildCanvas();
    printf("  Object %d modified successfully.\n", id);
}

/* ═══════════════════════════════════════════════════════════
 *  SECTION 9 – Main menu
 * ═══════════════════════════════════════════════════════════ */

void printMenu(void) {
    printf("\n╔══════════════════════════════════╗\n");
    printf("║     2D GRAPHICS EDITOR           ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Display canvas               ║\n");
    printf("║  2. Add object                   ║\n");
    printf("║  3. Delete object                ║\n");
    printf("║  4. Modify object                ║\n");
    printf("║  5. List objects                 ║\n");
    printf("║  6. Clear all objects            ║\n");
    printf("║  0. Exit                         ║\n");
    printf("╚══════════════════════════════════╝\n");
    printf("Choice: ");
}

int main(void) {
    int choice;

    clearCanvas();
    printf("Welcome to the 2D Graphics Editor!\n");
    printf("Canvas: %d rows x %d cols  |  Characters: '*' (drawn) '_' (empty)\n",
           ROWS, COLS);

    do {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');  /* flush bad input */
            choice = -1;
        }

        switch (choice) {
            case 1: displayCanvas();    break;
            case 2: addObject();        break;
            case 3: deleteObject();     break;
            case 4: modifyObject();     break;
            case 5: listObjects();      break;
            case 6:
                objectCount = 0;
                clearCanvas();
                printf("  All objects cleared.\n");
                break;
            case 0:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
