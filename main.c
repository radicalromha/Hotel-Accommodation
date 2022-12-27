#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15

struct guest {
  int id;
  char name[MAX_NAME_LENGTH];
  char phone[MAX_PHONE_LENGTH];
  int room;
};

// Create a new guest in the database
void create_guest(sqlite3 *db, struct guest *guest) {
  char *sql = "INSERT INTO guests (name, phone, room) VALUES (?, ?, ?)";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, guest->name, -1, NULL);
    sqlite3_bind_text(stmt, 2, guest->phone, -1, NULL);
    sqlite3_bind_int(stmt, 3, guest->room);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
      fprintf(stderr, "Failed to create guest: %s\n", sqlite3_errmsg(db));
    }
  } else {
    fprintf(stderr, "Failed to prepare SQL statement: %s\n", sqlite3_errmsg(db));
  }
  sqlite3_finalize(stmt);
}

// Retrieve a guest from the database by ID
void get_guest(sqlite3 *db, int id, struct guest *guest) {
  char *sql = "SELECT * FROM guests WHERE id = ?";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      guest->id = sqlite3_column_int(stmt, 0);
      strcpy(guest->name, sqlite3_column_text(stmt, 1));
      strcpy(guest->phone, sqlite3_column_text(stmt, 2));
      guest->room = sqlite3_column_int(stmt, 3);
    } else {
      fprintf(stderr, "Guest not found\n");
    }
  } else {
    fprintf(stderr, "Failed to prepare SQL statement: %s\n", sqlite3_errmsg(db));
  }
  sqlite3_finalize(stmt);
}

// Update a guest in the database
void update_guest(sqlite3 *db, struct guest *guest) {
  char *sql = "UPDATE guests SET name = ?, phone = ?, room = ? WHERE id = ?";
  sqlite3_stmt *stmt
