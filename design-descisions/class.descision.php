<?php

Class Descision{
  public $name;
  public $category;
  public $relies_on;
  public $affects;
  public $options;
  public $sub_descisions;

  private $created_location;

  function __construct($name, $category = false){
    $this->name = $name;
    $this->category = $category;

    $this->options = array();
    $this->sub_descisions = array();
    $this->affects = array();
    $this->relies_on = array();
    $this->influences = array();

    DescisionRegistry::registerDescision($this);
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
    $this->relies_on[]= $descision;
    return $this;
  }

  /**
   * [isDependant description]
   * @return boolean
   */
  function isDependant(){
    return (count($this->relies_on) === 0);
  }

  /**
   * [dependsOn description]
   * @param  Descision $d [description]
   * @return boolean
   */
  function dependsOn(Descision $d){
    if(in_array($d, $this->sub_descisions))
      return true;

    if(in_array($d, $this->relies_on))
      return true;

    foreach($this->options as $v)
      if($v->dependsOn($d))
        return true;

    return false;
  }
}
