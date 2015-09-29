<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:36:37
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-29 09:50:38
 */

class Compiler{
	public $list;
	public $rank;
	public $included;

	function __construct($descisionList = array(), $optionList = array(), $debugInfo = false){
		$this->list = array_merge($descisionList, $optionList);
		$this->rank = array();
		$this->included = array();
		$this->debug_info = $debugInfo;
	}

	function compile(){
		$this->rank = array_map(function($v){
			return $v;
		}, $this->list);

		$this->resolve();

		$this->rank = array_filter($this->rank, function($v){
			return ($v instanceof Descision);
		});

		usort($this->rank, function($a, $b){
			
			if(!$a->isDependant() && !$b->isDependant()){
				return 0;
			}

			if($a->dependsOn($b)){
				if($b->dependsOn($a)){
					return 0;
				}else{
					return 1;
				}
			}else{
				if($b->dependsOn($a)){
					return 0;
				}else{
					return -1;
				}
			}
		});

		$tab = function($n, $str){
			echo str_repeat("\t", $n) . $str;
		};

		foreach($this->rank as $v){
			echo "DESCISION $v->name\n";

			foreach($v->relies_on as $r){
				if($r instanceof Descision)
					echo "\tRELIES ON $r->name\n";
				else
					echo "\tRELIES ON $r\n";
			}
		}
	}

	function resolve(){
		foreach($this->list as $k => $v){
			if($v instanceof Descision){
				foreach($v->relies_on as $l => $w){
					if(is_string($w))
						$v->relies_on[$l] = $this->find($w, $v);
				}
				foreach($v->affects as $l => $w){
					if(is_string($w))
						$v->affects[$l] = $this->find($w, $v);
				}
			}else if($v instanceof Option){
				foreach($v->relies_on as $l => $w){
					if(is_string($w))
						$v->relies_on[$l] = $this->find($w, $v);
				}
			}
		}
	}

	function is_included($name){
		return in_array($name, $this->included);
	}

	function find($name, $sourceObject){
		$list = array_filter($this->list, function($v){
			global $name; 

			if($v->name == $name)
				return true;

			return false;
		});

		if(count($list) === 1)
			return $list[0];

		// foreach($this->list as $v)
			// echo "'".$v->name."'\n";

		$list = array_filter($this->list, function($v){
			global $name; 

			echo $v->name . " ? " . $name . "\n";
			if($v->name == $name)
				return true;
			
			return false;
		});

		// print_r($this->debug_info);

		if(count($list) === 0 && VERBOSE){
			warning("Couldn't find element '$name' \n\tlinked by $sourceObject->name\n\tin " . $this->debug_info[$sourceObject->name] . "\n");
		}else if(VERBOSE){
			warning("Found multiple values for element '$name'");
		}

		die();

		return $name;
	}
}