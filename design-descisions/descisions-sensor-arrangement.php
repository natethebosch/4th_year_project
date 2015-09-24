<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:17:27
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:25:36
 */

$sens_arranagement = new Descision("Sensor Arrangement");
$sens_arranagement
	->addOption(function(){
		$opt = new Option("Single sensor");
		
		$opt
			->addPro("One sensor to control");

		return $opt;
	})
	->addOption(function(){
		$opt = new Option("Array of sensors");

		$opt
			->addPro("Slower scan speed required");

		return $opt;
	})
	->addReliesOn("Sensor speed");