<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:27:54
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:36:29
 */

class DescisionRegistry{

	static $options = array();
	static $descisions = array();

	static function registerOption($option){
		DescisionRegistry::$options[] = $option;
	}

	static function registerDescision($descision){
		DescisionRegistry::$descisions[] = $descision;
	}
}