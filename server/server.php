<?php

$data = file_get_contents("data"); 
$convert = explode("\n", $data); 

$objArray = array();
$ballArray = array();
$robotArray = array();
$obsArray = array();

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
  }

  $objArray['r'] = $robotArray;
  $objArray['b'] = $ballArray;
  $objArray['o'] = $obsArray;

}

echo json_encode($objArray);

?>
