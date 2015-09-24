<?php

Class Descision{
  public $name;
  public $category;
  public $relies_on;
  public $_affects;
  public $options;
  public $sub_descisions;
  public $_relies;

  function __construct($name, $category = false){
    $this->name = $name;
    $this->category = $category;

    $this->options = array();
    $this->sub_descisions = array();
    $this->affects = array();
    $this->relies = array();
    $this->influences = array();

    Compiler::registerDescision($this);
  }

  function addOption($option){
    if(is_callable($option))
        $option = call_user_func($option);
    if(is_string($option) || $option instanceof Option)
        $this->options[] = $option;
    else
        if(VERBOSE){
            $debug = debug_backtrace()[1];
            warning("null option added to Descision " . $this->name . " in " . $debug['file'] . " " . $debug['function']  . " on line " . $debug['line']);
        }
    return $this;
  }

  function addSubDescision(Descision $descision){
    $this->sub_descisions[] = $descision;
    return $this;
  }

  function addAffects($descision){
    $this->affects[]= $descision;
    return $this;
  }
  function addReliesOn($descision){
    $this->relies[]= $descision;
    return $this;
  }
}
