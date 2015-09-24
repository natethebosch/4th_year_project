<?php

class Option{
  public $name;
  public $pros;
  public $cons;

  function __construct($name){
    $this->name = $name;

    $this->pros = array();
    $this->cons = array();
  }

  function addPro($string){
    $this->pros[] = $string;
  }

  function addCon($string){ 
    $this->pros[] = $string;
  }
}
