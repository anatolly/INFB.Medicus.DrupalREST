<?php


/**
* список объявленных типов REST
*/
function medicus_rest_overview_page() {
  $entity_info_list = medicus_rest_entity_info(); 
  
  $output = '';
  
  foreach($entity_info_list as $entity_type => $entity_info) {
    $uri = medicus_rest_uri($entity_type);
    $items[] = l($entity_info['label'], $uri['path']);
  }
  $output .= theme('item_list', array('title' => 'Medicus REST entity types', 'type' => 'ul', 'items' => $items));
  return $output;
}


/**
* список всех объектов REST по заданному типу 
*/
function medicus_rest_entity_list_page($rest_type) {
  $rest_data = medicus_rest_filter_query($rest_type);
  
  $i=0;
  
  $uri_prefix = current(medicus_rest_uri(medicus_type_rest2drupal($rest_type)));
  
  $output .= l('Add', $uri_prefix . '/add');
  
  foreach($rest_data as $item) {
    $rows[$i]['id'] = l($item->id, $uri_prefix . '/' . $item->id);
    $rows[$i]['name'] = l($item->Name, $uri_prefix . '/' . $item->id);
    $rows[$i]['op'] = l('Edit', $uri_prefix . '/' . $item->id . '/edit') . ' | ' . l('Delete', $uri_prefix . '/' . $item->id . '/delete');
    $i++;
  }
  
  $output .= theme('table', array('header' => array('id', 'Name', 'Operations'), 'rows' => $rows));
  
//   krumo($data); //die; 
  return $output;
}


/**
* Форма просмотра Drupal REST Entity
*/
function medicus_rest_medicus_rest_view($rest_type, $entity, $view_mode = 'full') {
  $entity_type = 'medicus_rest_'.$rest_type;
//   krumo($entity_type, $entity); die;
//   $entities = current(entity_load('medicus_rest_'.$rest_type, array($entity_id)));

  $output = entity_view($entity_type , array($entity), $view_mode, NULL, TRUE);
//   $output .= 'xxx';
  return $output;
}


/**
* Враппер для формы редактирования
*/
function medicus_rest_medicus_rest_form_wrapper($entity) {
  // Include the forms file from the Product module.
  return drupal_get_form('medicus_rest_medicus_rest_form', $entity);
}

/**
* Форма редактирования Drupal REST Entity
*/
function medicus_rest_medicus_rest_form($form, &$form_state, $entity) {

//   krumo($entity); 
  if(!is_object($entity)) {
    $entity_type = 'medicus_rest_' . $entity;
    $entity = new stdclass();
    $entity->entity_type = $entity_type;
    $entity->is_new = true;
  }
  
  $entity_property_info = entity_get_property_info($entity->entity_type);
  
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity->entity_type,
  );
  
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $entity->entity_id,
  );
  
  foreach($entity_property_info['properties'] as $property_id => $property_data) {
    switch($property_id) {
      case 'id':
        break;
      
      default: 
        $form[$property_id] = array(
          '#type' => 'textfield',
          '#title' => $property_data['label'],
          '#default_value' => isset($entity->$property_id) ? $entity->$property_id : NULL,
//           '#maxlength' => 128,
//           '#required' => TRUE,
//           '#weight' => -10,
        );
        break;
    }
    
  }

  $form_state['entity'] = $entity;

//   $langcode = entity_language('commerce_product', $product);
// 
//   if (empty($langcode)) {
//     $langcode = LANGUAGE_NONE;
//   }
  
//   // Simply use default language
//   $form['language'] = array(
//     '#type' => 'value',
//     '#value' => $langcode,
//   );

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array_merge($submit, array('medicus_rest_medicus_rest_form_submit')),
  );
  
  return $form;
}


function medicus_rest_medicus_rest_form_submit($form, &$form_state) {
  global $user;

  $entity = &$form_state['entity'];

  $entity_property_info = entity_get_property_info($entity->entity_type);

  foreach($entity_property_info['properties'] as $property_id => $property_data) {
    switch($property_id) {
      case 'id':
        break;
      
      default: 
        if(isset($form_state['values'][$property_id])) {
          $entity->$property_id = $form_state['values'][$property_id];
        }
        break;
    }
  }
//   // Notify field widgets.
//   field_attach_submit($entity->entity_type, $entity, $form, $form_state);

  entity_save($entity->entity_type, $entity);

  // Redirect based on the button clicked.
  drupal_set_message('Entity saved.');
}


function medicus_rest_medicus_rest_delete_confirm($form, &$form_state, $entity) {
  $form['#entity'] = $entity;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['entity_type'] = array('#type' => 'value', '#value' => $entity->entity_type);
  $form['entity_id'] = array('#type' => 'value', '#value' => $entity->entity_id);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $entity->entity_id)),
    'node/' . $entity->entity_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}


function medicus_rest_medicus_rest_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $entity = entity_load($form_state['values']['entity_type'], $form_state['values']['entity_id']);
    entity_delete($form_state['values']['entity_type'], $form_state['values']['entity_id']);
//     cache_clear_all();
    watchdog('medicus_rest', '@type: deleted %title.', array('@type' => $entity->entity_type, '%title' => $entity->entity_id));
    drupal_set_message(t('@type %title has been deleted.', array('@type' => $entity->entity_type, '%title' => $entity->entity_id)));
  }
  $form_state['redirect'] = '<front>';
}
