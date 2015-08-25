<?php
/*
 * This script is used by jquery to put data into the form
*/
session_start();
include_once './inc/config.php';
include 'functions.php';

$dsn = 'mysql:host='.DB_HOST.';dbname='.DBNAME;
$dbh = new PDO($dsn, LOGIN, PASS);
$tool = new Toolkit;

$soa = array();             // SOA record
$nsrecords = array();       // zone entries
$i = 0;                     // record index

 if(isset($_SESSION['userlogin'])) {
   /*
    * Let's get some SOA information here
   */
    $q = $dbh->prepare("SELECT domain,ttl,admin_contact,master_dns FROM domains WHERE id = ".$_POST['id']);
    $q->execute();
    $res = $q->fetch();
    $soa['domain'] = $res['domain'];
    $soa['ttl'] = $res['ttl'];
    $soa['admin_contact'] = $res['admin_contact'];
    $soa['master_dns'] = $res['master_dns'];
    /*
     * Now we can gather domain records
    */
    $q = $dbh->prepare("SELECT subdomain,type,ip FROM subdomains WHERE domain_id = ".$_GET['id']);
    $q->execute();
    while($res = $q->fetch()) {
      $nsrecords[$res['subdomain']] = array($res['type'] => $res['ip']);
      $i++;
    }
    print_r($nsrecords);
 }
?>
