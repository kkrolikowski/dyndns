<?php
/*
 * Database connection
*/
	define('DBNAME', 'dyndns');
	define('LOGIN', 'dyndns');
	define('PASS', 'xxx');
	define('DB_HOST', 'localhost');
/*
 * DNS settings
*/
	define('HOSTMASTER', 'kkrolikowski.gmail.com.');
	define('MASTERDNS', 'pazuzu.lan.');
	define('TTL', '86400');
	define('DNS_SERVERS', serialize(array('pazuzu.lan.', 'styx.lan.')));
	define('MX_SERVER', '10 styx.lan.');
/*
 * Other settings
*/
	define('SMARTY_LIB', 'Smarty.class.php');
	define('ADM_EMAIL', 'dyndns@bsdaemon.pl');
?>
