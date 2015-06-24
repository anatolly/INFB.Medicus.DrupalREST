<?php

function medicus_rest_entity_type_list() {
  
  $path = medicus_rest_admin_path();

  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => '1'));
  $rows = array();
  
  $medicus_rest_entity_info = medicus_rest_entity_info();
  
  $i=0;
  
  $actions = array("edit" => "Edit", "delete" => "Delete");

  foreach($medicus_rest_entity_info as $entity_type => $item) {
  
    $rows[$i]['entity_type']=l($item['label'], "{$path}/{$entity_type}");
    
    foreach ($actions as $action_key => $action_title) {
      $operations[] = l($action_title, "{$path}/{$entity_type}/$action_key");
    }
    
    $rows[$i]['operations'] = implode(' | ', $operations);
    
    $i++;
  }

  $build['entity_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}