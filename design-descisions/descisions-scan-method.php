<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:14:53
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:17:15
 */

$scan = new Descisions("Scan Method");
$scan
	->addOption(new Option("Stop and Go"));
	->addOption(new Option("Continuous"));