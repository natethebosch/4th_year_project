<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:02:47
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:32:07
 */

$power = new Descision("Power");
$power
	->addOption(function(){
		$opt = new Option("Battery");
		$opt
			->addReliesOn("Weather")
			->addCon("Poor performance in cold weather")
			->addPro("No wired power required")
			->addPro("Easy setup");

		return $opt;
	})
	->addOption(function(){
		$opt = new Option("120VAC");
		$opt
			->addReliesOn("Power Access")
			->addPro("Reliable");

		return $opt;
	});