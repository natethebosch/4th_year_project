<?php
/**
 * functions.php
 * Nate Bosscher
 * 2016-Feb-25
 */

/**
 * Gets the directory contents recursively and
 * returns an array
 */
function scanDirRecursive($dir){
	// output list
	$list = array();

	// scan directory
	foreach(scandir($dir) as $k => $v){
		// skip hidden folders
		if($v[0] == ".")
			continue;
		if(is_dir($dir . "/" . $v)){
			foreach(scanDirRecursive($dir . "/" . $v) as $l => $w)
				$list[] = $w;		
		}else{
			$list[] = $dir . "/" . $v;
		}
	}

	return $list;
}
