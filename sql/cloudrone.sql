-- MySQL dump 10.14  Distrib 5.5.33a-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: cloudronedb
-- ------------------------------------------------------
-- Server version	5.5.33a-MariaDB-log

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
-- Table structure for table `drone_ownership`
--

DROP TABLE IF EXISTS `drone_ownership`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `drone_ownership` (
  `user` varchar(50) NOT NULL,
  `drone` int(11) NOT NULL,
  KEY `user` (`user`),
  KEY `drone` (`drone`),
  CONSTRAINT `drone_ownership_ibfk_1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `drone_ownership_ibfk_2` FOREIGN KEY (`drone`) REFERENCES `drones` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `drone_ownership`
--

LOCK TABLES `drone_ownership` WRITE;
/*!40000 ALTER TABLE `drone_ownership` DISABLE KEYS */;
INSERT INTO `drone_ownership` VALUES ('test',0),('test',1);
/*!40000 ALTER TABLE `drone_ownership` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `drones`
--

DROP TABLE IF EXISTS `drones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `drones` (
  `id` int(11) NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  `model` varchar(80) NOT NULL,
  `location` varchar(200) NOT NULL,
  `state` int(11) NOT NULL,
  `driver` varchar(200) NOT NULL,
  `map` varchar(200) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `state` (`state`),
  CONSTRAINT `drones_ibfk_1` FOREIGN KEY (`state`) REFERENCES `states` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `drones`
--

LOCK TABLES `drones` WRITE;
/*!40000 ALTER TABLE `drones` DISABLE KEYS */;
INSERT INTO `drones` VALUES (0,'TestDroneDist','ardrone2','RSATU',1,'/home/walkindude/catkin_ws/record/flight_2013-10-19-14-40-12.bag',''),(1,'TestDroneObj','ardrone2','ISA',1,'/home/walkindude/catkin_ws/record/flight1_2013-10-19-14-40-12.bag','');
/*!40000 ALTER TABLE `drones` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `show_all`
--

DROP TABLE IF EXISTS `show_all`;
/*!50001 DROP VIEW IF EXISTS `show_all`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `show_all` (
  `id` tinyint NOT NULL,
  `name` tinyint NOT NULL,
  `model` tinyint NOT NULL,
  `location` tinyint NOT NULL,
  `driver` tinyint NOT NULL,
  `map` tinyint NOT NULL,
  `state` tinyint NOT NULL,
  `user` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `state_set`
--

DROP TABLE IF EXISTS `state_set`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `state_set` (
  `cstate` int(11) NOT NULL,
  `nstate` int(11) NOT NULL,
  PRIMARY KEY (`cstate`,`nstate`),
  KEY `nstate` (`nstate`),
  CONSTRAINT `state_set_ibfk_1` FOREIGN KEY (`cstate`) REFERENCES `states` (`id`),
  CONSTRAINT `state_set_ibfk_2` FOREIGN KEY (`nstate`) REFERENCES `states` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `state_set`
--

LOCK TABLES `state_set` WRITE;
/*!40000 ALTER TABLE `state_set` DISABLE KEYS */;
INSERT INTO `state_set` VALUES (0,1),(1,0),(1,2),(2,0),(2,3),(3,0),(3,1);
/*!40000 ALTER TABLE `state_set` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `states`
--

DROP TABLE IF EXISTS `states`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `states` (
  `id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `states`
--

LOCK TABLES `states` WRITE;
/*!40000 ALTER TABLE `states` DISABLE KEYS */;
INSERT INTO `states` VALUES (0,'Free'),(1,'Selected'),(2,'OnTask'),(3,'TaskCompleted');
/*!40000 ALTER TABLE `states` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` varchar(50) NOT NULL,
  `password` varchar(200) NOT NULL,
  `signed` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES ('test','7e07495675d45c4e94969485ccc8e57a',1),('test1','5a105e8b9d40e1329780d62ea2265d8a',0),('test2','ad0234829205b9033196ba818f7a872b',0);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `view1`
--

DROP TABLE IF EXISTS `view1`;
/*!50001 DROP VIEW IF EXISTS `view1`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `view1` (
  `id` tinyint NOT NULL,
  `password` tinyint NOT NULL,
  `signed` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Final view structure for view `show_all`
--

/*!50001 DROP TABLE IF EXISTS `show_all`*/;
/*!50001 DROP VIEW IF EXISTS `show_all`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `show_all` AS select `d`.`id` AS `id`,`d`.`name` AS `name`,`d`.`model` AS `model`,`d`.`location` AS `location`,`d`.`driver` AS `driver`,`d`.`map` AS `map`,`d`.`state` AS `state`,`do`.`user` AS `user` from (`drones` `d` left join `drone_ownership` `do` on((`d`.`id` = `do`.`drone`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `view1`
--

/*!50001 DROP TABLE IF EXISTS `view1`*/;
/*!50001 DROP VIEW IF EXISTS `view1`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `view1` AS select `users`.`id` AS `id`,`users`.`password` AS `password`,`users`.`signed` AS `signed` from `users` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-10-26 16:28:45
