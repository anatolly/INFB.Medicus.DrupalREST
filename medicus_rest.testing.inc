<?php

function medicus_rest_testing_page() {
//   $entity = entity_load('medicus_wsdata_probedata1',array(1,2));
//   $entity = entity_load('medicus_rest_probedata1', 5);
//   krumo($entity); // die;
// //   echo drupal_render($entity);
//   return 'xxx';

//   $query = new EntityFieldQuery();
//   $query
//     ->entityCondition('entity_type', 'node')
//     ->propertyCondition('nid', 1 ,'>');
//   
//   $result = $query->execute();
//   
//   krumo($result); 
// // 
  
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'medicus_rest_probedata1')
    ->propertyCondition('id', 6, '>=')
    ->propertyCondition('Name', '%lala%', 'like')
    ;
  
  $result = $query->execute();
  
  krumo($result); 
  
  return "fin"; //die;
}