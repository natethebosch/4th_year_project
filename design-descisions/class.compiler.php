<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:27:54
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:30:10
 */

class Compiler{

	static $options = array();
	static $descisions = array();

	static function registerOption($option){
		Compiler::$options[] = $option;
	}

	static function registerDescision($descision){
		Compiler::$descisions[] = $descision;
	}

	static function compile(){

	}
}