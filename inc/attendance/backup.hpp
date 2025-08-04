#ifndef BACKUP_HPP
#define BACKUP_HPP

#include <attendance/Database.hpp>
#include <fstream>

extern void backupCreate(Database& db);
extern void backupRestore(Database& db);

#endif
