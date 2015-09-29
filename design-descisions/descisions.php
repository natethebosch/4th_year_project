<?php
/**
 * Descisions.php
 * A list of descisions for our 4th year project
 * Use to compile a heirachy of descisions
 * Nate Bosscher (c) 2015
 */

// get the framework
include __dir__."/include.php";

include __dir__."/descisions-power.php";
include __dir__."/descisions-processor.php";
include __dir__."/descisions-scan-method.php";
include __dir__."/descisions-sensor-arrangement.php";
include __dir__."/descisions-setup.php";
include __dir__."/descisions-software.php";
include __dir__."/descisions-input.php";

$cost = new Descision("Cost");

if(VERBOSE){
	$compiler = new Compiler(DescisionRegistry::$options, DescisionRegistry::$descisions, DescisionRegistry::$debug);
}else{
	$compiler = new Compiler(DescisionRegistry::$options, DescisionRegistry::$descisions);
}

$compiler->compile();