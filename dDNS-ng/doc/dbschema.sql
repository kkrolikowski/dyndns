-- MySQL dump 10.11
--
-- Host: localhost    Database: dyndns
-- ------------------------------------------------------
-- Server version	5.0.51a-24+lenny5-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `domains`
--

DROP TABLE IF EXISTS `domains`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `domains` (
  `id` int(11) NOT NULL auto_increment,
  `domain` varchar(64) NOT NULL,
  `status` varchar(7) NOT NULL default '',
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `mailqueue`
--

DROP TABLE IF EXISTS `mailqueue`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `mailqueue` (
  `id` int(11) NOT NULL auto_increment,
  `mailto` varchar(64) character set utf8 collate utf8_unicode_ci NOT NULL,
  `mailfrom` varchar(64) character set utf8 collate utf8_unicode_ci NOT NULL,
  `subject` varchar(128) character set utf8 collate utf8_unicode_ci NOT NULL,
  `reply_to` varchar(64) character set utf8 collate utf8_unicode_ci NOT NULL,
  `x_mailer` varchar(64) character set utf8 collate utf8_unicode_ci NOT NULL,
  `message` text character set utf8 collate utf8_unicode_ci NOT NULL,
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `user_log`
--

DROP TABLE IF EXISTS `user_log`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `user_log` (
  `id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `ip` varchar(16) NOT NULL,
  `date` varchar(30) NOT NULL,
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `users` (
  `id` int(11) NOT NULL auto_increment,
  `login` varchar(16) collate utf8_unicode_ci NOT NULL,
  `pass` varchar(36) collate utf8_unicode_ci NOT NULL,
  `role` varchar(5) collate utf8_unicode_ci NOT NULL,
  `active` int(11) NOT NULL default '0',
  `name` varchar(64) collate utf8_unicode_ci NOT NULL,
  `email` varchar(64) collate utf8_unicode_ci NOT NULL,
  `activate` varchar(24) collate utf8_unicode_ci NOT NULL,
  `subdomain` varchar(64) collate utf8_unicode_ci NOT NULL,
  `ip` varchar(16) collate utf8_unicode_ci NOT NULL,
  `lastupdate` varchar(30) collate utf8_unicode_ci NOT NULL,
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=73 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
SET character_set_client = @saved_cs_client;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-06-26  7:39:01
