<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 19:36:37
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 20:04:26
 */

class Compiler{
	public $list;
	public $rank;
	public $included;

	function __construct($descisionList = array(), $optionList = array()){
		$this->list = array_merge($descisionList, $optionList);
		$this->rank = array();
		$this->included = array();
	}

	function compile(){
		$this->rank = array_map(function($v){
			return array(
				"item" => $v,
				"rank" => 0
			);
		}, $this->list);
		$this->resolve();

		foreach($this->list as $k => $v){
			if($v instanceof Descision){
				
			}
		}
	}

	function resolve(){
		foreach($this->list as $k => $v){
			if($v instanceof Descision){
				foreach($v->relies_on as $l => $w){
					if(is_string($w))
						$v->relies_on[$l] = $this->find($w);
				}
				foreach($v->affects as $l => $w){
					if(is_string($w))
						$v->affects[$l] = $this->find($w);
				}
			}else if($v instanceof Option){
				foreach($v->relies_on as $l => $w){
					if(is_string($w))
						$v->relies_on[$l] = $this->find($w);
				}
			}
		}
	}

	function is_included($name){
		return in_array($name, $this->included);
	}

	function find($name){
		$list = array_filter($this->list, function($v){
			if($v->name === $name)
				return true;
			return false;
		});

		if(count($list) === 1)
			return $list[0];

		if(count($list) === 0 && VERBOSE){
			warning("Couldn't find element '$name'");
		}else if(VERBOSE){
			warning("Found multiple values for element '$name'");
		}

		return $name;
	}
}