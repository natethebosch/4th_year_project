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
  }

  function addPro($string){
    $this->pros[] = $string;
  }

  function addCon($string){ 
    $this->pros[] = $string;
  }

  function addReliesOn($string, $choice = false){
    $this->relies[] = array("name" => $string, "option" => $choice);
  }
}
