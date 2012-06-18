DROP FUNCTION IF EXISTS mur3_32;
CREATE FUNCTION mur3_32 RETURNS INTEGER SONAME "murmur3_mysql_udf.so";
SELECT
  if(MUR3_32("") = 0, "pass", "fail"),
  if(MUR3_32("0") = 3530670207, "pass", "fail"), 
  if(MUR3_32("01") = 1642882560, "pass", "fail"), 
  if(MUR3_32("012") = 3966566284, "pass", "fail"), 
  if(MUR3_32("0123") = 3558446240, "pass", "fail"), 
  if(MUR3_32("01234") = 433070448, "pass", "fail"), 
  if(MUR3_32("I will not buy this record, it is scratched.") = 2832214938, "pass", "fail");
