<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:05:46
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:14:12
 */

$setup = new Descision("Setup");
{
	$se_ability = new Descision("Technician Ability");
	$se_ability
		->addOption(function(){
			$opt = new Option("Fully Qualified");
			$opt
				->addPro("More advanced setup requirements")
				->addCon("Less users will be able to access this technician");

			return $opt;
		})
		->addOption(function(){
			$opt = new Option("Intermediate");
			$opt
				->addPro("Allows for some technical setup requirements");

			return $opt;
		})->addOption(function(){
			$opt = new Option("None");
			$opt
				->addPro("Everyone can setup the apparatus")
				->addCon("Simplified pacakaging/setup proceedure");

			return $opt;
		});

	$setup->addSubDescision($se_ability);
}