<?php

class Option{
  public $name;
  public $pros;
  public $cons;
  public $relies;

  function __construct($name){
    $this->name = $name;

    $this->pros = array();
    $this->cons = array();
    $this->relies = array();

    Compiler::registerOption($this);
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
    $this->relies[] = array("name" => $string, "option" => $choice);
    return $this;
  }
}
