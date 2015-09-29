<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:27:54
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-29 09:30:09
 */

class DescisionRegistry{

	static $options = array();
	static $descisions = array();
	static $debug = array();

	static function registerOption($option){
		DescisionRegistry::$options[] = $option;
		if(VERBOSE){
			$bt = debug_backtrace()[1];
			DescisionRegistry::$debug[$option->name] = basename($bt['file']) . ":line-" . $bt['line'];
		}
	}

	static function registerDescision($descision){
		DescisionRegistry::$descisions[] = $descision;
		if(VERBOSE){
			$bt = debug_backtrace()[1];
			DescisionRegistry::$debug[$descision->name] = basename($bt['file']) . ":line-" . $bt['line'];
		}
	}
}