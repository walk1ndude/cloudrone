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
  PRIMARY KEY (`user`,`drone`),
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
INSERT INTO `drones` VALUES (0,'TestDroneDist','ardrone2','RSATU',0,'/media/f3/TestDroneDist.bag',''),(1,'TestDroneObj','ardrone2','ISA',0,'/media/f3/TestDroneObj.bag',''),(2,'TestDroneMinDist','ardrone2','RSATU',0,'/media/f3/TestDroneMinDist.bag',''),(3,'TestDroneMaxDist','ardrone2','RSATU',0,'/media/f3/TestDroneMaxDist.bag',''),(4,'TestDroneDist2','ardrone2','RSATU',0,'/media/f3/TestDroneDist.bag','');
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
  `task` varchar(20) DEFAULT NULL,
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
INSERT INTO `state_set` VALUES (0,1,NULL),(1,0,NULL),(1,1,NULL),(1,2,NULL),(2,0,NULL),(2,2,NULL),(2,3,'start'),(3,0,NULL),(3,1,NULL),(3,4,'finish'),(3,5,'pause'),(4,1,NULL),(4,3,'start'),(5,3,'resume');
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
INSERT INTO `states` VALUES (0,'Free'),(1,'Selected'),(2,'TaskGiven'),(3,'OnTask'),(4,'TaskCompleted'),(5,'TaskPaused');
/*!40000 ALTER TABLE `states` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `task_queue`
--

DROP TABLE IF EXISTS `task_queue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_queue` (
  `drone` int(11) NOT NULL,
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `command` varchar(100) NOT NULL,
  PRIMARY KEY (`drone`,`number`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `task_queue`
--

LOCK TABLES `task_queue` WRITE;
/*!40000 ALTER TABLE `task_queue` DISABLE KEYS */;
INSERT INTO `task_queue` VALUES (1,3,'setInitialReachDist 0.2'),(1,2,'setMaxControl 10.2'),(1,1,'autoInit 300 200 1000 0.5'),(2,7,'land'),(2,6,'goto 0 0 1 0'),(2,5,'setStayTime 4'),(2,4,'setStayWithinDist 0.5'),(2,3,'setInitialReachDist 0.2'),(2,2,'setMaxControl 10.2'),(2,1,'autoInit 300 200 1000 0.5'),(0,7,'land'),(0,6,'goto 0 0 1 0'),(0,5,'setStayTime 4'),(0,4,'setStayWithinDist 0.5'),(0,3,'setInitialReachDist 0.2'),(0,2,'setMaxControl 10.2'),(0,1,'autoInit 300 200 1000 0.5'),(1,4,'setStayWithinDist 0.5'),(1,5,'setStayTime 4'),(1,6,'goto 0 0 1 0'),(1,7,'land'),(3,1,'autoInit 300 200 1000 0.5'),(3,2,'setMaxControl 10.2'),(3,3,'setInitialReachDist 0.2'),(3,4,'setStayWithinDist 0.5'),(3,5,'setStayTime 4'),(3,6,'goto 0 0 1 0'),(3,7,'land');
/*!40000 ALTER TABLE `task_queue` ENABLE KEYS */;
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
-- Dumping routines for database 'cloudronedb'
--
/*!50003 DROP FUNCTION IF EXISTS `signUser` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`cloudrone`@`localhost` FUNCTION `signUser`(in_id varchar(50), in_password varchar(200), isPageUpdate bool) RETURNS int(11)
    MODIFIES SQL DATA
    DETERMINISTIC
begin
	if exists(select id from users where id = in_id)
	then begin
		if exists(select id from users where id = in_id and signed = true)
		then begin
			if in_password = ''
			then begin
				if (isPageUpdate)
				then begin
					return 1;
				end;
				else begin
					update users set signed = false where id = in_id;
					return 2;
				end;
				end if;
			end;
			elseif exists(select id from users where id = in_id and password = md5(in_password))
			then begin
				return 1;
			end;
			else begin
				return 0;
			end;
			end if;
		end;
		else begin
			if exists(select id from users where id = in_id and password = md5(in_password))
			then begin
				update users set signed = true where id = in_id;
				return 1;
			end;
			else begin
				return 0;
			end;
			end if;
		end;
		end if;
	end; 
	else begin
		return 0;
	end;
	end if;
end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

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

-- Dump completed on 2013-11-19 18:39:03
