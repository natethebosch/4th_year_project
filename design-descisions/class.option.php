<?php

class Option{
  public $name;
  public $pros;
  public $cons;
  public $relies_on;

  private $created_location;

  function __construct($name){
    $this->name = $name;

    $this->pros = array();
    $this->cons = array();
    $this->relies_on = array();

    DescisionRegistry::registerOption($this);
  }

  function addPro($string){
    $this->pros[] = $string;
    return $this;
  }

  function addCon($string){ 
    $this->pros[] = $string;
    return $this;
  }

  function addReliesOn($string, $choice = false){
    $this->relies_on[] = array("name" => $string, "option" => $choice);
    return $this;
  }

  function dependsOn(Descision $d){
    return in_array($d, $this->relies_on);
  }
}
