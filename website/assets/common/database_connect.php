<?php
mysql_connect(DB_HOST, DB_USER, DB_PASS);
@mysql_select_db(DB_DB) or die( "Unable to select database");
?>