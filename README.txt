= SciDB (Database Module + FuzzySQL) module =

* The toolbox enables database management from Scilab
  and handling with fuzzy databases.
  For specifications, see SEP "Database module + Fuzzy SQL"
* to compile and run the toolbox you need3-rd party libraries (Qt) are
  distributed under LGPL conditions: QtCore4, QtSql4 and database driver
  libraries

== Windows installation ==

The dependencies are provided into the scidb distribution.

== Linux installation == 
SciDB is considering that QtSQL and the extensions are installed by the 
package manager.

For example, under Debian/Ubuntu, it is:
libqt4-sql, libqt4-sql-sqlite or libqt4-sql-odbc
SciDb will use these libraries for building and at runtime.

== Authors ==
Main author:
Igor GRIDCHYN, igridchyn@gmail.com
Module developped in the context of the Google Summer of Code 2010.

With help from Allan Cornet and Sylvestre Ledru
