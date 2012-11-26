<?php

$data = file_get_contents("data"); 
$convert = explode("\n", $data); 

$objArray = array();
$ballArray = array();
$robotArray = array();
$robotBArray = array();
$enemyArray = array();
$obsArray = array();
$targetArray = array();

foreach($convert as $key=>$element){

  if($element == null){
    unset($convert[$key]);
  }

  //check type
  if(strpbrk($element, 'r')){
    //It's a robot
    $robotArray[] = $element;
  }else if(strpbrk($element, 'b')){
    //It's a ball
    $ballArray[] = $element;
  }else if(strpbrk($element, 'o')){
    //It's obstacle
    $obsArray[] = $element;
  }else if(strpbrk($element, 't')){
    //It's target
    $targetArray[] = $element;
  }else if(strpbrk($element, 'n')){
    //It's locationB of the robot
    $robotBArray[] = $element;
  }else if(strpbrk($element, 'e')){
    //It's enemy robot location
    $enemyArray[] = $element;
  }

  $objArray['r'] = $robotArray;
  $objArray['n'] = $robotBArray;
  $objArray['b'] = $ballArray;
  $objArray['o'] = $obsArray;
  $objArray['t'] = $targetArray;
  $objArray['e'] = $enemyArray;
}

echo json_encode($objArray);

?>
