<?php

function warning($str){
    echo "Warning: $str\n";
}

define("VERBOSE", true);

  include __dir__."/class.compiler.php";
  include __dir__."/class.option.php";
  include __dir__."/class.descision.php";
