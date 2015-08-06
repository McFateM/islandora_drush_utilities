<?php

/**
 * Some available drush functions...
 * 
 * drush_log('Log an event using drush', 'warning');
 * drush_set_error('Set an error with drush.');
 * t('Translate strings with drush');
 * drush_print('Print to command line with drush');
 * drush_print_table($rows, TRUE); //print a command line table with drush
 * drush_confirm('Are you sure you want to continue?', $indent = 0); //Add drush confirmation
 * 
 */

/**
 * Implements hook_drush_help().
 */
function idu_drush_help($command) {
  switch ($command) {
    case 'drush:iduTest':
      return t('Just a temporary test feature.');
    case 'drush:iduFix':
      return t("Attempts to apply the specified 'Fix' to one or more existing PIDs.");
    case 'drush:iduUpdate':
      return t("Attempts to apply the specified 'Update' to one or more existing PIDs.");
    case 'drush:iduSpecial':
      return t("Attempts to apply the specified 'Special' operation to a single existing PID.");
    case 'drush:iduHandle':
      return t("Attempts to apply the specified Handle operation to one or more existing PIDs.");
  }
  return;
}

/**
 * Implements hook_drush_command().
 */
function idu_drush_command( ) {
  $items = array();

  $items['iduTest'] = array(
    'description' => t('Just a temporary test feature.'),
    'examples' => array(
      'Example' => 'drush iduTest',
    ),
    'aliases' => array('iduTest'),
  );

  $items['iduFix'] = array(
    'description' => t("Attempts to apply the specified 'fix' to one or more existing PIDs."),
    'arguments'   => array(
      'first' => t("First PID to process.  The namespace portion of this PID defines the default primary namespace.  This parameter may be specified as 'namespace:first-last' in place of the --repeat option."),
      'fix'   => "Completes the name of the drush_idu_fix_NAME function to be called for each processed object.  Available Fix functions are:

      Clean -
        Removes all unnecessary datastreams from an existing object.

      Derivatives -
        Attempts to regenerate ALL derivatives for an existing object.

      Purge -
        Deletes the object.  Be careful with this one!

      PurgeDatastream -
        Deletes the --dsid datastream.  Be careful with this one too!

      PurgeFamily -
        Delete an object and all of its constituents!

      PurgeInactive -
        Deletes the specified objects if they are in an Inactive or Deleted
          state.

      ChangeText -
        Changes --find text to --replace either in general or within the
          value of a specified --xpath.

      DCTransform -
        Applies the specified self-transform to an object's MODS then
          applies a specified MODS-to-DC Transform.

      SelfTransform -
        Applies the specified --self self-transform to the specified
          --dsid datastream.

      Thumbnail -
        Applies the specified thumbnail image to an object.

      IndexSolr -
        Attempts to update an object's Solr index.

      Relax -
        Applies a 'relaxed' POLICY to an object to make it visible
          to all.

      MIMEFix -
        Corrects errant image/jpg and image/tif MIME types,
          changing them to image/jpeg and image/tiff.

      toPHPP -
        Migrates an object from the grinnell:generic collection
          to grinnell:phpp.

      toIR -
        Migrates an object from any collection to the grinnell:generic
          collection.

      TitleBookPage -
        Changes the label/title of a book page to
          'Parent Title - Page XXX'.

      isConstituentOf -
        Changes a literal isConstituentOf value to a proper
          attribute form.

      CompoundTN -
        Creates a CompoundTN datastream for a compound parent object.
          The new image datastream is constructed from up to four
          child object thumbnails.

      ConvertTIFtoJP2 -
        Converts an object OBJ datastream to a JP2 using options
          specified by --convert or --compress.

      CopyDatastream -
        Copies the named --dsid datastream to one named --replace, and if
          --self is specified, runs the self transform on the original
          datastream.

      MakeSibling -
        Attempts to make the specified object a sibling of an existing
          --sibling PID.

      MakeFamily -
        Converts the specified object to a compound parent and makes the
          original content a new child of that parent.

      UpdateDC -
        Ensures that no specified objects have an empty (or no) dc:date,
          dc:creator OR dc:source value.  Default values are 'Unspecified' and
          'Unknown'.

      PurgeElements -
        Removes all --xpath matched elements from the specified --dsid
          datastream.",

    ),
    'options' => array(
     // 'fedoraURL' => t("Specifies the URL to Fedora like http://repository7.grinnell.edu:8080/fedora. Default is the example shown here."),
     // 'username' => t("Specifies the Fedora repository user name. Default is 'fedoraAdmin'."),
     // 'password' => t("Specifies the password of the Fedora user. There is NO default."),
     // 'repeat'   => t("Specifies the total number of PIDs, beginning with 'first' to be processed.  Default is 1."),
      'dsid'     => t("Specifies the datastream ID in which to make changes."),
      'collection' => t("Specifies the target collection (may include a wildcard like '*test') from which objects will be selected."),
      'date'     => t("Specifies a dc:date value for the UpdateDC fix.  Default is 'Unspecified'."),
      'creator'  => t("Specifies a dc:creator value for the UpdateDC fix.  Default is 'Unknown'."),
      'source'   => t("Specifies the DC field from which dc:source is to be constructed.  Default is 'dc:creator'."),
      'xpath'    => t("Specifies a metadata XPath whenever one is needed. Note that it is wise to put your XPath in double quotes and use single quotes within where necessary."),
      'find'     => t("Specifies text to be found (and in some cases replaced)."),
      'replace'  => t("Specifies the text that 'find' will be replaced with."),
      'substitutionsFile' => t("In place of 'find' and 'replace', this option specifies the path to a 'csv' style file from which 'find, replace' pairs will be read."),
      'self'     => t("Specifies the path to the self-transform XSL file to be applied. Defaults to public://cleanup_mods.xsl."),
      'xsl'      => t("Specifies the path to the MODS-to-DC transform XSL file to be applied. Defaults to public://mods_to_dc_grinnell-modified-2014-09-24.xsl."),
      'image'    => t("Specifies the path to a jpeg image to be applied by the specified Fix operation.  Defaults to public://audio_thumbnail.jpg."),
      'mime'     => t("Specifies the OBJ MIME type of objects to process.  Defaults to 'image/tiff'."),
      'convert'  => t("Specifies a file containing ImageMagick 'convert' options for the ConvertTIFtoJP2 fix."),
      'compress' => t("Specifies a file containing Kakadu 'compress' options for the ConvertTIFtoJP2 fix."),
      'sibling'  => t("Specifies the PID of one existing child object in need of a sibling."),
      'parent'   => t("Specifies the PID of an existing parent object in need of a child."),
),
    'examples' => array(
      'Example' => "drush iduF grinnell:10000 ChangeText --dsid=MODS --xpath=\"/mods:mods/mods:relatedItem/mods:identifier[@type='uri']\"",
    ),
    'aliases' => array('iduF'),
  );

  $items['iduUpdate'] = array(
    'description' => t("Attempts to apply the specified 'Update' to one or more existing PIDs."),
    'arguments'   => array(
      'first'  => t("First PID to process.  The namespace portion of this PID defines the default primary namespace.  This parameter may be specified as 'namespace:first-last' in place of the --repeat option."),
      'update' => "Completes the name of the drush_idu_update_NAME function to be called for each processed object.  Available Update functions are:

      Dates -
        Reads the FOXML date entities shown here and maps the data to MODS RecordInfo elements.

        <foxml:property NAME=\"info:fedora/fedora-system:def/model#createdDate\" VALUE=\"2004-12-10T00:21:58.000Z\"/>
        <foxml:property NAME=\"info:fedora/fedora-system:def/view#lastModifiedDate\" VALUE=\"2005-01-20T22:46:07.506Z\"/>",
    ),

    'options' => array(
      'repeat'   => t("Specifies the total number of PIDs, beginning with 'first' to be processed.  Default is 1."),
    ),

    'examples' => array(
      'Example' => "drush iduU grinnell:10000 Dates",
    ),
    'aliases' => array('iduU'),
  );


  $items['iduHandle'] = array(
    'description' => t("Attempts to apply the specified Handle operation to one or more existing PIDs."),
    'arguments'   => array(
      'first'     => t("First PID to process.  The namespace portion of this PID defines the default primary namespace.  This parameter may be specified as 'namespace:first-last' in place of the --repeat option."),
      'handle_op' => "Completes the name of the drush_idu_Handle_NAME function to be called for each processed object.  Available Handle functions are:

           Create - Attempts to create a new handle for an existing object.
           Modify - Attempts to modify an existing handle for an existing object.",
    ),
    'options'  => array(
      'repeat' => t("Specifies the total number of PIDs, beginning with 'first' to be processed.  Default is 1."),
      'force'  => t("If specified, handle assignment will proceed even if the mods:identifier[@type='hdl'] element is not compatible with handle_op."),
    ),
    'examples' => array(
      'Example' => "drush iduH grinnell:10000 Create --repeat=100",
    ),
    'aliases' => array('iduH'),
  );

  $items['iduSpecial'] = array(
    'description' => t("Attempts to apply the specified 'Special' operation to a single, existing PID."),
    'arguments'   => array(
      'pidAlpha'  => t('The primary PID to be processed.  Example: grinnell:52.'),
      'operation' => t('Completes the name of the idu_drush_special_OPERATION function to be called. Available operations are: PrintRI, SingleCompound, FixArtist and FixStatus.'),
    ),
    'options'     => array(
      'secondary' => t('An optional secondary PID to be used in the process.  Defaults to NULL.'),
    ),
    'examples' => array(
      'Example' => "drush iduS grinnell:622 SingleCompound",
    ),
    'aliases' => array('iduS'),
  );

  return $items;
}

/**
 * Examine common iduX command arguments/options and set control variables for subsequent processing.
 *
 * @param string $pidSpec
 *   The range (or single) PID argument as specified in the iduX command.
 * @param int $last
 *   Returns the 'last' PID value based on the range specification, if any.  Note that $last may or may not be an existing object.
 * @param int $pidn
 *   Returns the integer PID number of 'range'.  Note that $pidn may or may not be an existing object.
 * @param int $range
 *   Returns 'last' - 'first' + 1, the total number of PIDs to address.
 * @param string $namespace
 *   Returns the namespace (without a colon!) associated with the PID numbers returned by the function.
 * @param bool $return
 *   If TRUE, the function returns an array of valid PID numbers.
 * @return array
 *   An array of valid PID numbers, without a namespace prefix, from the specified range.
 *
 */
function idu_drush_prep($pidSpec, &$last, &$pidn, &$range, &$namespace, $return=FALSE) {

  /* Get some necessary options!
  $fedoraURL = drush_get_option('fedoraURL', 'http://repository7.grinnell.edu:8080/fedora');
  $username = drush_get_option('username', 'fedoraAdmin');
  $password = drush_get_option('password', NULL);

  // Open the repository.
  $connection = new RepositoryConnection($fedoraURL, $username, $password);
  $connection->reuseConnection = TRUE;
  $repository = new FedoraRepository(
    new FedoraApi($connection),
    new SimpleCache());  */

  // Open a repository connection.
  $my_tuque = islandora_get_tuque_connection( );
  // $repository = $my_tuque->repository;

  // Test fetch the islandora:root object to check the connection.
  $pid = "islandora:root";
  if (!$fedora_object = islandora_object_load($pid)) {
    drush_log("Error: idu_drush_prep was unable to verify a repository connection!", 'error');
    return FALSE;
  } else {
    drush_log("Connection to the repository is complete.", 'status');
  }

  // Initialize some things.
  list($namespace, $pidN) = explode(':', $pidSpec);    // Break the PID spec into namespace and PID number.

  list($begin, $end) = explode('-', $pidN);          // Break $pidN into beginning and end of range, if necessary
  $pidn = intval($begin);

  if (is_null($end)) {
    $repeat = 0;
  } else {
    $repeat = intval($end) - intval($begin);
  }
  $range = $repeat + 1;

  // If $pidN is not an integer or if $end is null (there is no '-' separator defining a range)
  // then we have only one PID to process...do it!
  if (intval($pidN) === 0 || is_null($end)) {
    drush_log("Completed fetch of a single object PID.", 'status');
    return array($pidSpec);
  }

  /* If no namespace given, use the icg_namespace variable.  Otherwise, set the icg_namespace variable.
  if (is_null($namespace)) {
    $namespace = strstr(variable_get('icg_namespace', 'test'), ':', TRUE);
  } else {
    variable_set('icg_namespace', "$namespace:");
  } */

  // Calculate first and last PID numbers and set variables accordingly.
  variable_set('icg_first', $pidn);
  $last = $pidn + $repeat;
  variable_set('icg_last', $last);

  $start = date('H:i:s');
  drush_log("Starting operation for PIDs $pidn to $last at $start.", 'status');

  // Pre-fetch the specified PIDs if requested.
  if ($return) {
    drush_log("Fetching all valid object PIDs in the specified range.", 'status');
    $pids = idu_drush_getPIDs($namespace, $pidn, $last);
    drush_log("Completed fetch of " . count($pids) . " valid object PIDs.", 'status');
    return $pids;
  } else {
    return FALSE;
  }
}


/**
 * Prints a progress bar directly to STDOUT.
 *
 * Call with no values (NULLs) for $current and $range to initialize the progress bar.
 *
 * @param string $label
 *   Label to appear above the bar when initialized.
 * @param int|NULL $current
 *   The current counter (from 0 to $range) in the icg_first to icg_last progression.
 * @param int|NULL $range
 *   The total number of steps in the icg_first to icg_last progression.
 */
function idu_drush_print_progress($label, $current=NULL, $range=NULL) {
  static $green = "\033[1;32m";
  static $white = "\033[0;37m";

  if (is_null($current)) {
    $output = $green . "Progress: $label \n";
    print $output;
    return;
  }

  $ratio = ($current+1) / $range;
  $percentage = floor($ratio * 100) . '%';
  $columns = drush_get_context('DRUSH_COLUMNS', 80);
  // Subtract 10 characters for the percentage, brackets, spaces and arrow.
  $progress_columns = $columns - 10;
  // If ratio is 1 (complete), the > becomes a = to make a full bar.
  $arrow = ($ratio < 1) ? '>' : '=';
  // Print a new line if ratio is 1 (complete). Otherwise, use a CR.
  $line_ending = ($ratio < 1) ? "\r" : "\n";

  // Determine the current length of the progress string.
  $current_length = floor($ratio * $progress_columns);
  $progress_string = str_pad('', $current_length, "=");

  $output  = $green . '[';
  $output .= $progress_string . $arrow;
  $output .= str_pad('', $progress_columns - $current_length);
  $output .= ']';
  $output .= str_pad('', 5 - strlen($percentage)) . $percentage;
  $output .= $line_ending . $white;

  print $output;
}

/**
 * Drush idu_fix_* function to purge an object (be careful!) from the repository.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF <pid> Purge
 *
 * Note that if the health report indicates this object to have constituents, they will be marked
 * for subsequent removal in the pending_drush_commands.sh file.
 *
 * @param string $pid
 *   The PID to be purged.
 * @param array $options
 *   There are NO purge-specific options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_Purge($pid, $options=NULL) {
  list($ns, $r) = explode(':', $pid);
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Get this object's health from the available CSV data.  Verify that we have the correct object.
  if ($row = or_get($pid)) {
    if ($row['B'] === $pid) {
      list($nC, $constituents) = idu_get_count_and_content($row['H']);  // from the RI
      if ($nC > 0) {
        idu_drush_post_to_file("Purge existing children of object $pid.", NULL);
        $parts = explode(',', $constituents);
        foreach ($parts as $part) {
          if (preg_match('/^(\d+)$/', $part)) {
            idu_drush_post_to_file(NULL, "drush iduF $ns:$part Purge\n");
          } else if (preg_match('/^(\d+)\-(\d+)$/', $part, $matches)) {
            $num = intval($matches[2]) - intval($matches[1]);
            idu_drush_post_to_file(NULL, "drush iduF $ns:$matches[1] Purge --repeat=$num\n");
          }
        }
      }
    }
  }

  idu_repo( )->purgeObject($pid);
  return 1;
}

/**
 * Drush idu_fix_* function to relax POLICY on an existing object by moving the POLICY datastream out of the way.
 *
 * Invoke this function during iduC process using an option of the form:
 *   drush iduF <pid> Relax --find=<text string>
 *
 * If a --find option is specified the change will be made ONLY if the specified string exists
 * in the DC datastream, and if both --find and --dsid exist then only the specified DSID will be searched.
 *
 * @param int $r
 *   The PID number to operate on.
 * @param array $options
 *   An array of options.  In this case $options['find'] and $options['dsid'] may exist
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_Relax($pid, $options=NULL) {
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  if (isset($options)) {
    $find = (isset($options['find']) ? $options['find'] : NULL);
    $dsid = (isset($options['dsid']) ? $options['dsid'] : NULL);
  }

  if (isset($find)) {
    if (isset($dsid)) {
      $ds = $object[$dsid];
    } else {
      $ds = $object['DC'];
    }
    $content = $ds->content;
    if (!strpos($content, $find)) { return 0; }
  }

  if (isset($object['POLICY'])) {
    $newDS = icg_copy_datastream($object, 'POLICY', 'text/xml', 'X', 'Old_POLICY');
    $object->ingestDatastream($newDS);
    unset($object['POLICY']);
  } else {
    return 0;
  }

  return 1;
}

/**
 * Drush idu_fix_* function to change RELS-EXT and RELS-INT from text/xml (v6) MIME type application/rdf+xml (v7).
 *
 * Invoke this function during iduC process using an option of the form:
 *   --fix=RELSMIME
 *
 * @param int $r
 *   The PID number to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_RELSMIME($r) {
  die('idu_drush_fix_RELSMIME is disabled.  ');
  $pid = variable_get('icg_namespace').$r;

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $count = 0;


  if ($ds = $object->getDatastream('RELS-EXT')) {
    $mime = $ds->mimetype;
    if ($mime === 'text/xml') {
      $ds->mimetype = 'application/rdf+xml';
      ++ $count;
    }
  }

  if ($ds = $object->getDatastream('RELS-INT')) {
    $mime = $ds->mimetype;
    if ($mime === 'text/xml') {
      $ds->mimetype = 'application/rdf+xml';
      ++ $count;
    }
  }

  return $count;
}

/**
 * Drush idu_drush_fix_* function to correct image/tif and image/jpg MIME types, changing them to image/tiff or
 * image/jpeg as needed.
 *
 * Invoke this function using 'drush iduF <pid> MIMEFix'.
 *
 * @param string
 *   The PID number to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_MIMEFix($pid) {
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $count = 0;
  foreach ($object as $ds) {
    if ($ds->state != 'A') {
      continue;
    } else if ($ds->mimetype === 'image/tif') {
      $ds->mimetype = 'image/tiff';
      ++ $count;
    } else if ($ds->mimetype === 'image/jpg') {
      $ds->mimetype = 'image/jpeg';
      ++ $count;
    }
  }

  return $count;
}

/**
 * Drush idu_fix_* function to change the text of elements in the target datastream and xpath.
 *
 * Invoke this function via iduF and the 'ChangeText' operation parameter.  Something like this...
 *
 *   drush iduF grinnell:501 ChangeText --substitutionsFile="public://findReplace.csv" --dsid=MODS --xpath="/mods:mods/mods:description/mods:identifier[@type='uri']
 *
 * Applicable options include the following:
 *
 *   --substitutionsFile=<file path>  If this option is specified then --find and --replace are ignored and
 *       pairs of find => replace strings are pulled instead from the named file.  Default is NULL.
 *
 *   --find=<string>  If this option is specified, and --substitutionsFile is not, then the specified string
 *       is searched for in the target datastreams and replaced with values specified in --replace.
 *
 *   --replace=<string>  See --find above.
 *
 *   --dsid=<string>  This option specifies the ID of the datastream to be operated on in each object.
 *
 *   --xpath=<string>  This option, if present, specifies an xpath used to query the target datastream.  If specified,
 *       find => replace operations are restricted to the value returned by the xpath, so only the text() value of
 *       the xpath can be changed.  If --xpath is omitted it defaults to NULL and this specifies that the entire
 *       datastream be treated as a string and find => replace then operates on it as a string.  You can effectively
 *       alter the ‘structure’ of a datastream in this mode so BE VERY CAREFUL!
 *
 * @param string $pid
 *   The PID to operate on.
 * @param array $options
 *   An array of options.  In this case $options['findReplace'] should carry an associative array
 *   with one or more find => replace pairs.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_ChangeText($pid, $options) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Get the pre-processed find and replace options.

  $findReplace = $options['findReplace'];

  // Check for other options...

  $dsid = drush_get_option('dsid', 'MODS');
  $xpath = drush_get_option('xpath', NULL);

  $total = $cnt = 0;

  if ($ds = $object[$dsid]) {
    $count = 0;
    $content = $ds->content;

    foreach ($findReplace as $from => $to) {
      if (strpos($content, $from)) {
        if (is_null($xpath)) {
          $content = str_replace($from, $to, $content, $cnt);
          $count += $cnt;
        } else {
          $xml = new DOMDocument;
          $xml->loadXML($content);
          $dx = new DOMXPath($xml);
          $results = $dx->query($xpath);
          foreach ($results as $node) {
            $value = (string) $node->nodeValue;
            if (strpos(' ' . $value, $from)) {
              $node->nodeValue = str_replace($from, $to, $value, $cnt);
              $count += $cnt;
              $content = $xml->saveXML( );
            }
          }
        }
      }

    }

    if ($count > 0) {
      $total += $count;
      $object[$dsid]->setContentFromString($content);
    }
  }

  return $total;
}

/**
 * Callback function for drush iduSpecial.
 *
 * This callback runs a specified 'special' operation for ONE object.
 *
 * @param $pid
 *   The primary PID to process.
 * @param string $operation
 *   Specifies the 'special' operation to be performed by calling function idu_drush_special_$operation.
 */
function drush_idu_iduSpecial($pid, $operation) {

  // Get the operation parameter and make sure such a function exists...
  $function = 'idu_drush_special_'.$operation;
  if (!function_exists($function)) { die("The specified Special function $function does not exist!   "); }

  // Report some info.
  $start = date('H:i:s');
  drush_log("Starting iduSpecial '$operation' for PID $pid at $start.", 'status');

  $count = call_user_func($function, $pid);

  // Done.
  $finish = date('H:i:s');
  if ($count == 1) {
    drush_log("Successfully completed ONE $operation operation at $finish.", 'status');
  } else if ($count <> 1) {
    drush_log("Successfully completed $count operations at $finish.", 'status');
  } else {
    drush_log("The Special $operation was unsuccessful and returned NO valid results at $finish.", 'error');
  }

}

/**
 * Drush idu_fix_* function to regenerate derivatives.  @TODO: Fix me!
 *
 * Invoke this function using iduF <pid> Derivatives with --repeat if necessary.
 *
 * @param string $pid
 *   The object PID to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_Derivatives($pid) {

  // For any exception, report and skip this PID
  if (!$object = islandora_object_load($pid)) {
    drush_log("idu_drush_fix_Derivatives($pid) returned NO object!", 'error');
    return 0;
  }

  // Check for --mime option.  If present and the object has no OBJ or the OBJ MIME is not of the correct
  // type, then skip this object.
  $mime = drush_get_option('mime', NULL);
  if (!is_null($mime)) {
    if (!$obj = $object['OBJ']) { return 0; }
    $objMIME = $obj->mimetype;
    if ($mime != $objMIME) { return 0; }
  }

  $count = 0;
  $rels = $object->relationships->get(FEDORA_MODEL_URI, 'hasModel');
  $nCM = count($rels);
  if ($nCM != 1) { return 0; }
  $cModel = $rels[0]['object']['value'];

  // Create new derivatives.
  switch ($cModel) {
    case 'islandora:sp_basic_image' :
      if (module_exists('islandora_basic_image')) {
        islandora_basic_image_create_thumbnail($object, TRUE);
        islandora_basic_image_create_medium_size($object, TRUE);
        $count += 2;
      }
      if (module_exists('islandora_fits')) {
        $fit = islandora_fits_create_techmd($object, TRUE, array('source_dsid'=>'OBJ'));
        if ($fit['success']) { ++$count; }
      }
      break;
    case 'islandora:pageCModel' :
    case 'islandora:sp_large_image_cmodel' :
      if (module_exists('islandora_large_image')) {
        $count += 3;
        islandora_large_image_create_tn_derivative($object, TRUE);
        islandora_large_image_create_jpg_derivative($object, TRUE);
        islandora_large_image_create_jp2_derivative($object, TRUE);
      }
      if (module_exists('islandora_fits')) {
        $fit = islandora_fits_create_techmd($object, TRUE, array('source_dsid'=>'OBJ'));
        if ($fit['success']) { ++$count; }
      }
    break;
    case 'islandora:sp_pdf' :
      if (module_exists('islandora_pdf')) {
        $count += 3;
        islandora_pdf_add_fulltext_derivative($object, TRUE);
        islandora_pdf_add_preview_derivative($object, TRUE);
        islandora_pdf_add_tn_derivative($object, TRUE);
      }
      if (module_exists('islandora_fits')) {
        $fit = islandora_fits_create_techmd($object, TRUE, array('source_dsid'=>'OBJ'));
        if ($fit['success']) { ++$count; }
      }
      break;
    case 'islandora:sp-audioCModel' :
      if (module_exists('islandora_audio')) {
        $count += 2;
        islandora_audio_create_thumbnail($object, TRUE);
        islandora_audio_create_mp3($object, TRUE);
      }
      if (module_exists('islandora_fits')) {
        $fit = islandora_fits_create_techmd($object, TRUE, array('source_dsid'=>'OBJ'));
        if ($fit['success']) { ++$count; }
      }
      break;
  }

  return $count;
}

/**
 * Drush idu_fix_* function to apply MODS self-transform and MODS-to-DC transforms to an existing object.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF first DCTransform
 *
 * Applicable options are:
 *   --self=<path to the MODS self-transform XSL file>  Defaults to public://cleanup_mods.xsl
 *   --xsl=<path to the MODS-to-DC transform XSL file>  Defaults to public://mods_to_dc_grinnell-modified-2015-04-03.xsl
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_DCTransform($pid) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Check for an existing MODS... if none, we're done.
  if (!$mods = $object['MODS']) { return 0; }

  // Check for existing DC record... if not, we're done.
  if (!$dc = $object['DC']) { return 0; }

  // Get the MODS self-transform.
  $xsl = new DOMDocument( );
  $transform = drush_get_option('self', 'public://cleanup_mods.xsl');
  if (!$xsl->load($transform)) { die("Failed to load Transform file '$transform!'"); }
  $xslt = new XSLTProcessor( );
  $xslt->importStyleSheet($xsl);

  // Apply it to the MODS content.
  $document = DOMDocument::loadXml($object['MODS']->content);
  if ($doc = $xslt->transformToDoc($document)) {
    $after = $doc->saveXML();
    $object['MODS']->setContentFromString($after);
  }

  // Now, get the MODS-to-DC transform.
  $xsl = new DOMDocument( );
  $transform = drush_get_option('xsl', 'public://mods_to_dc_grinnell-modified-2015-04-03.xsl');
  if (!$xsl->load($transform)) { die("Failed to load Transform file '$transform!'"); }
  $xslt = new XSLTProcessor( );
  $xslt->importStyleSheet($xsl);

  // Apply the MODS-to-DC transform.
  $document = DOMDocument::loadXml($object['MODS']->content);
  if ($doc = $xslt->transformToDoc($document)) {
    $after = $doc->saveXML();
    $object['DC']->setContentFromString($after);
  }

  // Re-index this object in Solr.
  update_solr_index($pid);

  return 1;
}

/**
 * Drush idu_fix_* function to apply a specified self-transform existing object datastream.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF first SelfTransform
 *
 * Applicable options are:
 *   --self=<path to the self-transform XSL file>  Defaults to public://cleanup_mods.xsl
 *   --dsid=The target datastream ID such as 'MODS', 'MADS' or 'DC'.  Defaults to 'MODS'.
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_SelfTransform($pid) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Check that the specified datastream exists... if not, we're done.
  $dsid = drush_get_option('dsid', 'MODS');
  if (!$ds = $object[$dsid]) { return 0; }

  // Get the specified self-transform.
  $xsl = new DOMDocument( );
  $transform = drush_get_option('self', 'public://cleanup_mods.xsl');
  if (!$xsl->load($transform)) { die("Failed to load Transform file '$transform!'"); }
  $xslt = new XSLTProcessor( );
  $xslt->importStyleSheet($xsl);

  // Apply it to the $ds content.
  $document = DOMDocument::loadXml($object[$dsid]->content);
  if ($doc = $xslt->transformToDoc($document)) {
    $after = $doc->saveXML();
    $object[$dsid]->setContentFromString($after);
  }

  // Re-index this object in Solr.
  update_solr_index($pid);

  return 1;
}


/**
 * Returns an array of existing PIDs (numbers only, no namespace prefix) found in the specified namespace and range.
 *
 * @param string $ns
 *   The target namespace (without a colon) of the PIDs to be returned.
 * @param $first
 *   The first PID number to check.
 * @param $last
 *   The last PID number to check.
 * @return array
 *   The PID numbers of all objects found in the range. The variable icg_last is
 *   automatically set to largest number returned here.
 */
function idu_drush_getPIDs($ns, $first, $last) {
  $pids = array( );

  //  @TODO  Check the --collection option to restrict selection.  Defaults to *, ALL collections.

  // Loop through the repo looking for objects in the specified range.
  for ($largest=0, $i=$first, $notFound=0; $i<=$last; $i++) {
    $pid = "$ns:$i";
    try {
      $object = idu_repo( )->getObject($pid);
    } catch (Exception $e) {
      drush_log("Exception: Attempting to open $pid.  The exception message follows:", 'error');
      drush_log($e, 'status');
      $notFound++;
      continue;  // for any exception...skip this PID
    }
    $pids[] = $i;
    $largest = $i;
  }

  // If any PIDs were not found, report them.
  if ($notFound > 0) { drush_log("A total of $notFound PIDs in the specified range were NOT FOUND.", 'warning'); }
  // Set the icg_last value.
  variable_set('icg_last', $largest);

  return $pids;
}

/**
 * Write a comment and/or follow-up iduF (or similar command) to a script for use later on.
 *
 * @param string $comment
 * @param string $command
 */
function idu_drush_post_to_file($comment, $command, $path=NULL) {

  // if $path is NULL... Fetch the --script option.
  if (is_null($path)) {
    $script = drush_get_option('script', 'public://iduPendingOps.sh');
  } else {
    $script = $path;
  }

  // If there is a comment, prepend $comment with # and a timestamp.
  if (!is_null($comment)) {
    file_put_contents($script, "# $comment [" . date('Y-m-d H:i:s') . "] \n", FILE_APPEND);
  }

  // Now output the command as-is.
  if (!is_null($command)) {
    file_put_contents($script, "$command\n", FILE_APPEND);
  }
}

/**
 * Add a child object (page or constituent) to an existing list of children.
 *
 * @param string $kids
 *   Existing '[count] list' string.
 * @param integer $newKid
 *   New ID to be added, if necessary.
 * @return bool|string
 *   Returns an updated '[count] list' or FALSE for no change.
 */
function idu_drush_add_child_to_list($kids, $newKid) {
  $new = intval($newKid);
  $range = array( );
  list($count, $content) = idu_get_count_and_content($kids);
  $children = explode(',', $content);
  if ($count == 0) {                                    // first child...simply add it
    return "[1] $newKid";
  } else if (in_array($newKid, $children)) {            // child already in the list, do nothing
    return FALSE;
  } else {                                              // child not already present, unless it falls in an existing range
    foreach ($children as $child) {
      if (preg_match('/(\d+)-(\d+)/', $child, $range)) {   // we have a range of values...see if $newKid is within
        if (($new >= intval($range[1])) && ($new <= intval($range[2]))) { return FALSE; }
      }
    }

    // If we reach this point then we know $newKid is not yet accounted for...add it.
    ++ $count;
    $last = end($children);
    if (preg_match('/(\d+)-(\d+)/', $last, $range)) {   // we have a range of values in the last position, add to it
      if (intval($range[2]) == $new-1) {                   // extend the range
        $range[2] = $new;
        $updated = "[$count] ";
        for ($i = 0; $i < count($children)-1; $i++) {
          $updated .= "$children[$i],";
        }
        $updated .= $range[1].'-'.$range[2];
      } else {                                         // new child does not extend the range, add it to the end as a single
        $updated = "[$count] $content,$newKid";
      }
    } else {                                           // last child is a single...add another single or make it a range
      if (intval($last) == $new-1) {
        $updated = "[$count] $content-$newKid";        // make this a range
      } else {
        $updated = "[$count] $content,$newKid";        // add as another single
      }
    }

    return $updated;
  }
}


/**
 * Drush idu_drush_special_* function to print an object's resource index (RI) values.
 *
 * Invoke this function via iduS and the 'PrintRI' operation parameter.  Something like this...
 *
 *   drush iduS grinnell:501 PrintRI
 *
 * @param string $pid
 *   The PID of the object to operate on.
 * @return int
 *   The number of successful operations performed, or zero if none.
 */
function idu_drush_special_PrintRI($pid) {
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Get the cModel of the $pid.  Change the cModel of the $compound to be the same.
  $rels = $object->relationships->get(FEDORA_MODEL_URI, 'hasModel');
  $cModel = $rels[0]['object']['value'];

  // Get constituents from the RI.
  $pages = NULL;
  if ($cModel === 'islandora:bookCModel') {
    $pages = islandora_paged_content_get_pages($object);
  } else if ($cModel === 'islandora:compoundCModel') {
    $pages = islandora_compound_object_get_parts($object->id);
  }

  // Print the page (constituent) information...
  if ($pages) {
    $count = count($pages);
    drush_log("Object $pid has a CModel of '$cModel' and $count RI constituents.", 'info');
    foreach ($pages as $page => $info) {
      drush_log("Constituent information for: $page", 'info');
      drush_log(print_r($info, TRUE), 'info');
    }
  } else {
    drush_log("Object $pid has a CModel of '$cModel' and no RI constituents.", 'info');
  }
}

/**
 * Drush idu_fix_* function to move an errant rdf:resource literal VALUE to the appropriate attribute.
 *
 * Invoke this function via iduF and the 'isConstituentOf' operation parameter.  Something like this...
 *
 *   drush iduF grinnell:501 isConstituentOf
 *
 * @param string $pid
 *   The PID to operate on.
 * @param array $options
 *   An array of passed options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_isConstituentOf($pid, $options) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  if ($rels = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isConstituentOf')) {
    $literal = $rels[0]['object']['literal'];
    if ($literal) {
      $value = $rels[0]['object']['value'];
      // drush_log("Object $pid has a LITERAL isConstituentOf value of '$value'!", 'warning');
      $object->relationships->remove(FEDORA_RELS_EXT_URI, 'isConstituentOf');
      $object->relationships->add(FEDORA_RELS_EXT_URI, 'isConstituentOf', $value);
      return 1;
    }
    return 0;
  }
}

/**
 * Drush idu_fix_* function to change a bookPage label to read as "Parent Title - Page X'.
 *
 * Invoke this function via iduF and the 'TitleBookPage' operation parameter.  Something like this...
 *
 *   drush iduF grinnell:336-500 TitleBookPage
 *
 * @param string $pid
 *   The PID to operate on.
 * @param array $options
 *   An array of passed options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_TitleBookPage($pid, $options) {

  // Fetch the object.
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Check for hasModel = pageCModel
  $rels = $object->relationships->get(FEDORA_MODEL_URI, 'hasModel');
  $cModel = $rels[0]['object']['value'];
  if ($cModel != 'islandora:pageCModel') { return 0; }

  // Fetch the parent object ID and page number
  $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'isPageOf');
  $parent = $rels[0]['object']['value'];
  $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'isPageNumber');
  $page = $rels[0]['object']['value'];

  // Open the parent
  try {
    $pObject = idu_repo( )->getObject($parent);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Apply the parent title and page number to the object label.
  $pTitle = trim($pObject->label);
  $title = $object->label = substr($pTitle, 0, 240)." - Page $page";

  // Now update the DC record's dc:title.
  $dc = new DOMDocument( );
  $dc->loadXML($object['DC']->content);
  $xpath = new DOMXpath($dc);
  if ($nodes = $xpath->query("/oai_dc:dc/dc:title")) {
    foreach ($nodes as $node) {
      $node->nodeValue = $title;
      $object['DC']->setContentFromString($dc->saveXML( ));
      return 1;
    }
  }

  return 0;
}

/**
 * Drush idu_fix_* function to move an object from the grinnell:generic collection to grinnell:phpp.
 *
 * Invoke this function via iduF and the 'toPHPP' operation parameter.  Something like this...
 *
 *   drush iduF grinnell:501 toPHPP
 *
 * @param string $pid
 *   The PID to operate on.
 * @param array $options
 *   An array of passed options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_toPHPP($pid, $options) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  if ($rels = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection')) {
    $object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'grinnell:generic');
    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'grinnell:phpp');
    return 1;
  }
  return 0;
}

/**
 * Drush idu_fix_* function to move an object from any collection to the grinnell:generic collection.
 *
 * Invoke this function via iduF and the 'toIR' operation parameter.  Something like this...
 *
 *   drush iduF grinnell:4937 toIR
 *
 * @param string $pid
 *   The PID to operate on.
 * @param array $options
 *   An array of passed options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_toIR($pid, $options) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  if ($rels = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection')) {
    $object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
    $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'grinnell:generic');
    return 1;
  }
  return 0;
}


/**
 * Callback function for drush iduTest.
 *
 */
function drush_idu_iduTest( ) {
  variable_set('orLimit', iduLimit( ));
  $pid = 'grinnell:52';

  // $record = array('A'=>100, 'B'=>'grinnell:100', 'C'=>'A');
  // or_set($pid, $record);

  $codes = array('A'=>'RED', 'C'=>'GREEN');
  or_set_codes($pid, $codes);

  // Done.
  $finish = date('H:i:s');
  drush_log("Completed iduTest at $finish.", 'status');

}

/**
 * Drush idu_fix_* function to create a CompoundTN datastream for a compound parent.
 *
 * This function uses ImageMagick 'montage' to create a new datastream from the TN
 * datastreams of up to 4 child objects.
 *
 * Invoke this function using iduF <pid> CompoundTN.
 *
 * @param string $pid
 *   The object PID to operate on.
 * @return int
 *   The number of CompoundTN operations performed, or zero if none.
 */
function idu_drush_fix_CompoundTN($pid, $preOpts=NULL) {

  try {
    $object = idu_repo()->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Get a list of all the child objects.
  $children = islandora_compound_object_get_parts($object->id);
  if (empty($children)) {
    drush_log("Object $pid has NO children!", 'warning');
    return 0;
  }

  // Check for --convert option.
  $convert = drush_get_option('convert', NULL);

  // If --convert is present, read options from the specified file and build a string of arguments.
  if ($convert) {
    $handle = fopen($convert, "r");
    $options = "";
    if ($handle) {
      while (($line = fgets($handle)) !== FALSE) {
        $options .= trim($line) . ' ';
      }
    }
    else {
      drush_log("Cannot open the options file '$convert'!", 'error');
      return 0;
    }
    fclose($handle);
  }

  $dsid = 'CompoundTN';

  // Build a list of the child TN datastreams to build the new image from.
  $i = 0;
  $limit = min(count($children), 4);
  $command = "montage $options ";

  while ($i < $limit) {
    $command .= "http://repository7.grinnell.edu:8080/fedora/objects/" . $children[$i] . "/datastreams/TN/content ";
    $i++;
  }

  $cTName = "/var/www/drupal7/sites/all/modules/custom/idu/compoundTN.png";
  $command .= "-geometry +4+4 -frame 5 -tile ${i}x1 -background none $cTName ";

  // OK, the montage command is built...run it.
  drush_log("CompoundTN command is: '$command' for object $pid.", 'status');
  $output = array();
  exec($command, $output, $return_status);

  // Add an ellipsis if necessary...
  if (count($children) > $limit) {
    $temp = "/var/www/drupal7/sites/all/modules/custom/idu/temp.png";
    if (rename($cTName, $temp)) {
      $command = "montage $options $temp /var/www/drupal7/sites/all/modules/custom/idu/3dots.png ";
      $command .= "-geometry +4+4 -tile 2x1 -background none $cTName ";
      exec($command, $output, $return_status);
      drush_log("An ellipsis (...) was added to the CompoundTN image for object $pid.", 'status');
    } else {
      drush_log("Error...Image $cTName could not be renamed to $temp.");
      return 0;
    }
  }

  // If a new compoundTN.png exists, save its contents to the object's CompoundTN datastream.
  if (file_exists($cTName)) {
    $datastream = isset($object[$dsid]) ? $object[$dsid] : $object->constructDatastream($dsid,"M");
    $datastream->label = 'Compound Parent Thumbnail Montage';
    $datastream->mimeType = 'image/png';
    $datastream->setContentFromFile($cTName);
    $object->ingestDatastream($datastream);
    drush_log("CompoundTN call is complete and the PNG result has been added to object $pid.", 'status');
    // Clean up any old temporary files in the working directory.
    if (file_exists($cTName)) { unlink($cTName); }
    return 1;
  } else {
    drush_log("CompoundTN call did NOT yield a new PNG image!", 'error');
    return 0;
  }
}


/**
 * Drush idu_fix_* function to purge an object's illegitimate (be careful!) children from the repository.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF first PurgeFamily
 *
 * Note that if the health report indicates this object to have constituents, they will be removed from the
 * repository immediately! The script first checks to be sure the object has ONLY ONE legitimate content datastream
 * from the old system.
 *
 * @param string $pid
 *   The PID to be purged.
 * @param array $options
 *   There are NO purge-specific options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_PurgeFamily($pid, $options=NULL) {
  list($ns, $r) = explode(':', $pid);
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $count = 0;

  // Get this object's health from the available object_record data.  Verify that we have the correct object.
  if ($row = or_get($pid)) {
    if ($row['B'] === $pid) {
      list($nC, $constituents) = idu_get_count_and_content($row['H']);  // from the RI
      list($nK, $contentDS) = idu_get_count_and_content($row['K']);     // content datastreams
      if ($nC > 0 && $nK == 1) {
        $parts = explode(',', $constituents);
        foreach ($parts as $part) {
          if (preg_match('/^(\d+)$/', $part)) {
            idu_drush_post_to_file("PurgeFamily removing child '$ns:$part' from object '$pid'.", NULL);
            idu_repo( )->purgeObject("$ns:$part");
            ++ $count;
          } else if (preg_match('/^(\d+)\-(\d+)$/', $part, $matches)) {
            for ($i=intval($matches[1]); $i<=intval($matches[2]); $i++) {
              idu_drush_post_to_file("PurgeFamily removing child '$ns:$i' from object '$pid'.", NULL);
              idu_repo( )->purgeObject("$ns:$i");
              ++ $count;
            }
          }
        }
      }
    }
  }

  return $count;
}

/**
 * Drush idu_fix_* function to purge inactive and deleted objects (be careful!) from the repository.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF first PurgeInactive
 *
 * @param string $pid
 *   The PID to be purged.
 * @param array $options
 *   There are NO purge-specific options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_PurgeInactive($pid, $options=NULL) {
  list($ns, $r) = explode(':', $pid);
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Get this object's health from the available object_record data.  Verify that we have the correct object.
  if ($row = or_get($pid)) {
    if ($row['B'] === $pid && $row['C'] != 'A') {  // got the right PID and it's not active?
      idu_drush_post_to_file("PurgeInactive removing object '$pid'.", NULL);
      idu_repo( )->purgeObject($pid);
      return 1;
    }
  }

  return 0;
}

/**
 * Drush idu_fix_* function to re-index an existing object in Solr.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF <pids> IndexSolr
 *
 * There are no extra options.
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_IndexSolr($pid) {
  update_solr_index($pid);
  return 1;
}

/**
 * Drush idu_fix_* function to add a specific thumbnail image datastream to
 * an existing object.
 *
 * Invoke this function via iduF and the 'Thumbnail' operation parameter.  Something like this...
 *
 *   drush iduF grinnell:501 Thumbnail --image="public://audio_thumbnail.jpg"
 *
 * Only the --image option is applicable to this operation and it defaults to public://audio_thumbnail.jpg
 * if omitted.
 *
 * @param string $pid
 *   The PID to operate on.
 * @param array $options
 *   An array of options.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_Thumbnail($pid, $options=NULL) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $image = drush_get_option('image', 'public://audio_thumbnail.jpg');
  if (!$path = drupal_realpath($image)) { return 0; }

  // Already have a thumbnail, skip it.
  if ($ds = $object['TN']) { return 0; }

  // No TN here, create one and attempt to set content from the specified file.
  $ds = $object->constructDatastream('TN');
  if (!$ds) { return 0; }

  $ds->label = 'thumbnail';
  $ds->mimetype = 'image/jpeg';
  $ds->setContentFromFile($path);
  if (!$object->ingestDatastream($ds)) { return 0; }

  return 1;
}

/**
 * Callback function for drush iduHandle, alias iduH.
 *
 * This callback runs the prescribed Handle command for a given PID with an optional --repeat
 * to touch subsequent objects in the same namespace.  Functions called must be of the form...
 *     $count = drush_idu_handle_OPERATION($pid);
 *
 * @param string $first
 *   First PID to process.
 * @param string $handle_op
 *   The Handle operation to be performed.  This string completes the name of an existing idu_drush_handle_* function.
 */
function drush_idu_iduHandle($first, $handle_op) {

  // Get the Handle operation parameter...
  $function = 'idu_drush_Handle_'.$handle_op;
  if (!function_exists($function)) { drush_log("The specified Handle function '$function' does not exist!", 'error'); }

  if (!$pids = idu_drush_prep($first, $last, $pidn, $range, $ns, TRUE)) { return; }     // Get the PIDs and initialize the loop...
  $count = 0;

  idu_drush_print_progress("iduHandle - $handle_op");
  for ($i=0, $p=$pidn; $i<$range; $i++, $p++) {
    $pid ="$ns:$p";
    if (in_array($p, $pids)) {
      $count += call_user_func($function, $pid);
    }
    idu_drush_post_to_file("iduHandle - $handle_op: $pid complete.", NULL, 'public://iduHandle_progress.log');
    idu_drush_print_progress("iduFix - $handle_op", $i, $range);
  }

  // If one or more handles were created there should be a batch file to process.  Do that now.
  if ($count > 0) {
    $fileName = 'temporary://'.H_BATCH;
    $logFile = 'temporary://'.H_LOG;
    if (!file_exists($fileName)) { die("Could not find existing batch file $fileName!  "); }
    if (!$file = fopen($fileName, 'r')) { die("Could not open existing batch file $fileName to read!  "); }

    $path = drupal_realpath($fileName);
    $log = drupal_realpath($logFile);

    // Build a call to the batch file and run it.
    $output = array( );
    $command = H_BINDIR."/hdl-genericbatch $path $log.tmp -verbose";
    exec($command, $output, $return_status);

    // Keep a timestamped copy of the log file.
    if (file_exists("$log.tmp")) {
      $newLog = 'temporary://'.H_LOG.time( );
      file_put_contents($newLog, file_get_contents($log));
      file_put_contents($newLog, file_get_contents("$log.tmp"), FILE_APPEND);
      drush_log("Batch call to the Handle System is complete. A copy of the composite log file is stored in '$newLog'.", 'status');
    } else {
      drush_log("Batch call to the Handle System is complete but no log file was generated?", 'error');
    }
  }

  // Delete the temporary batch command file and log file.
  if (file_exists($fileName)) { unlink($fileName); }
  if (file_exists($logFile)) { unlink($logFile); }
  if (file_exists("$log.tmp")) { unlink("$log.tmp"); }

  // Done.
  $finish = date('H:i:s');
  drush_log("Completed $count 'iduHandle - $handle_op' operations at $finish. \n", 'status');
}

/**
 * idu_drush_Handle_Create($pid);
 *
 * Generates a 'generic batch' command file for the creation/assignment of one Handle for object $pid.
 *
 * The file will be created and run in H_SVRDIR as H_BATCH (see constants.inc in the idu module).
 */
function idu_drush_Handle_Create($pid) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $force = drush_get_option('force', FALSE);
  return idu_handle_create_or_modify($object, 'CREATE', $force);
  }

/**
 * idu_drush_Handle_Modify($pid);
 *
 * Generates a 'generic batch' command file to modify and assign the Handle identifier for object $pid.
 *
 * The file will be created and run in H_SVRDIR as H_BATCH (see constants.inc in the idu module).
 */
function idu_drush_Handle_Modify($pid) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $force = drush_get_option('force', FALSE);
  return idu_handle_create_or_modify($object, 'MODIFY', $force);
}

/**
 * Drush idu_fix_* function to convert a TIF to JP2 using a specified ImageMagick 'convert' or Kakadu 'compress' syntax.
 *
 * Invoke this function using iduF <pid> ConvertTIFtoJP2 --convert=file, or --compress=file, where "file" specifies
 * a text file filled with either ImageMagick convert or Kakadu compress arguments.
 *
 * @param string $pid
 *   The object PID to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_ConvertTIFtoJP2($pid, $preOpts=NULL) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Check for --convert or --compress option.  One must be present, but not both!
  $convert = drush_get_option('convert', NULL);
  $compress = drush_get_option('compress', NULL);
  if (is_null($convert) && is_null($compress)) {
    drush_log("Either the --convert or --compress option MUST be specified!", 'error');
    return 0;
  } else if ($convert && is_null($compress)) {
    $file = $convert;
  } else if ($compress && is_null($convert)) {
    $file = $compress;
  } else {
    drush_log("You cannot specify BOTH --convert and --compress in the same command!", 'error');
    return 0;
  }

  // Read options from the specified file and build a string of arguments.
  $handle = fopen($file, "r");
  $options = "";
  if ($handle) {
    while (($line = fgets($handle)) !== false) { $options .= trim($line).' '; }
  } else {
    drush_log("Cannot open the options file '$file'!", 'error');
    return 0;
  }
  fclose($handle);

  // Build the target file names.
  list($ns,$pidn) = explode(':',$pid);
  $name ="$ns$pidn";
  $tifName = "/temp/$name.tif";
  $jp2Name = "/temp/$name.jp2";

  // Fetch the object's OBJ datastream, presumably a TIFF, for conversion.
  $obj = $object['OBJ']->content;
  file_put_contents($tifName,$obj);

  // If --compress is specified we need to uncompress the TIFf before invoking Kakadu.  Do that now.
  if ($compress) {
    file_put_contents("/temp/$name.compressed.tif",$obj);
    /* $kdu_expand = variable_get('islandora_kakadu_url', 'kdu_expand');
    $command = "$kdu_expand -i /temp/$name.compressed.tif -o $tifName"; */
    $command = "convert /temp/$name.compressed.tif -compress None $tifName 2> /dev/null";
    exec($command, $output, $return_status);
  }

  // Build the convert or compress command and run it.
  if ($convert) {
    $command = "convert $tifName $options $jp2Name";
  } else if ($compress) {
    $kdu_compress = variable_get('islandora_kakadu_url', 'kdu_compress');
    $command = "$kdu_compress -i $tifName -o $jp2Name $options -record /temp/kakadu.log";
  }
  // drush_log("ConvertTIFtoJP2 command is: '$command' for object $pid.", 'status');
  $output = array( );
  exec($command, $output, $return_status);

  // If a new temp.jp2 file was created, save its contents to the object's JP2 datastream.
  if (file_exists($jp2Name)) {
    $object['JP2']->setContentFromFile($jp2Name);
    // drush_log("ConvertTIFtoJP2 call is complete and the JP2 result has been updated for object $pid.", 'status');
    /* Clean up any old temporary files in the working directory.
    if (file_exists($tifName)) { unlink($tifName); }
    if (file_exists($jp2Name)) { unlink($jp2Name); } */
    return 1;
  } else {
    drush_log("ConvertTIFtoJP2 call did NOT yield a new JP2 image!", 'error');
    return 0;
  }
}

/**
 * Drush idu_fix_* function removes unnecessary datastreams from an existing object.
 *
 * Invoke this function during iduC process using an option of the form:
 *   drush iduF <pid> Clean
 *
 * @param int $r
 *   The PID number to operate on.
 * @param array $options
 *   An array of options.  None are expected in this case.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_Clean($pid, $options=NULL) {
  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Below are the DS labels for all datastreams to keep.

  $keep = array('DC','OBJ','RELS-EXT','MODS','MADS','TN','JP2','JPG','PDF',
    'MEDIUM_SIZE','MP4','PREVIEW','FULL_TEXT','PROXY_MP3','OCR','islandora_workflow',
    'POLICY','Old_POLICY');

  // Loop through the datastreams and mark as Deleted any that are NOT in $keep
  // and not already marked as Deleted.

  $count = 0;
  foreach ($object as $ds) {
    $id = $ds->id;
    if (!in_array($id,$keep)) {
      ++ $count;
      unset($object[$id]);
//  if (($ds->state != 'D') && (!in_array($id,$keep))) {
//    $ds->state = 'D';
    }
  }
  if ($count > 0) { update_solr_index($pid); }

  return $count;
}


/**
 * Drush idu_fix_* function to copy a specified datastream to a new label.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF $pids --dsid=OLD --replace=NEW  --self=public://cleanup_mods.xsl
 *
 * Applicable options are:
 *   --self=<path to the self-transform XSL file>  Defaults to NULL.
 *   --dsid=The existing datastream ID such as 'MODS', 'MADS' or 'DC'.  Defaults to 'MODS'.
 *   --replace=The new (copy) datastream ID such as 'MODS_BACKUP'.  Defaults to NULL.
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_CopyDatastream($pid) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Check that the specified datastream exists... if not, we're done.
  $dsid = drush_get_option('dsid', 'MODS');
  if (!$ds = $object[$dsid]) { return 0; }

  if (!$replace = drush_get_option('replace', NULL)) {
    drush_log("A --replace datastream name MUST be specified in the command.", 'error');
    return 0;
  };

  // Make the new datastream copy.  Assume it is a text/xml DS.
  $newDS = icg_copy_datastream($object, $dsid, 'text/xml', 'X', $replace);
  $object->ingestDatastream($newDS);

  // Get the specified self-transform, if any.
  if ($transform = drush_get_option('self', NULL)) {
    $xsl = new DOMDocument();
    $transform = drush_get_option('self', NULL);
    if (!$xsl->load($transform)) {
      die("Failed to load Transform file '$transform!'");
    }
    $xslt = new XSLTProcessor();
    $xslt->importStyleSheet($xsl);

    // Apply it to the $ds content.
    $document = DOMDocument::loadXml($object[$dsid]->content);
    if ($doc = $xslt->transformToDoc($document)) {
      $after = $doc->saveXML();
      $object[$dsid]->setContentFromString($after);
    }

    // Re-index this object in Solr.
    update_solr_index($pid);
  }

  return 1;
}


/**
 * Drush idu_fix_* function to add a sibling object to an existing parent.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF $pids MakeSibling --sibling=<sibling PID>
 *
 * Applicable options are:
 *   --sibling=<PID of one child object>  Defaults to NULL.
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of children added, or zero if none.
 */
function idu_drush_fix_MakeSibling($pid) {

  // Fetch the intended sibling object...

  try {
    $object = idu_repo()->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Fetch the existing sibling object...

  if (!$siblingPID = drush_get_option('sibling', NULL)) {
    drush_log('No existing sibling PID specified!', 'error');
    return 0;
  }

  try {
    $sibling = idu_repo()->getObject($siblingPID);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $data = islandora_compound_object_retrieve_compound_info($sibling);

  // Determine the parent PID and sequence number for the new addition...

  $parentPID = $data['parent_pid'];
  $num = $data['child_count'];
  $next = $num + 1;

  // If we have a legitimate parent with at least one child, open the object's
  // RELS-EXT and attach it to the parent.

  if ($parentPID && $num > 0) {
    $rx = $object['RELS-EXT'];
    $xml = new DOMDocument;
    $xml->loadXML($rx->content);
    $dx = new DOMXPath($xml);

    // Find the rdf:RDF/rdf:Description...the parent of our new elements.

    $parents = $dx->query("/rdf:RDF/rdf:Description");
    $element1 = $xml->createElement("fedora:isConstituentOf");
    $attr = $xml->createAttribute("rdf:resource");
    $attr->value = "info:fedora/" . $parentPID;
    $element1->appendChild($attr);
    $parent_PID = str_replace(":", "_", $parentPID);
    $element2 = $xml->createElement("islandora:isSequenceNumberOf" . $parent_PID, $next);

    foreach ($parents as $parent) {
      $parent->appendChild($element1);
      $parent->appendChild($element2);
      $content = $xml->saveXML();
      $object['RELS-EXT']->setContentFromString($content);
      return 1;
      break;
    }
    return 0;
  }
  return 0;
}

/**
 * Callback function for drush iduFix, alias iduF.
 *
 * This callback runs the prescribed fix for a given PID with an optional --repeat to touch
 * subsequent objects in the same namespace.  Functions called must be of the form...
 *     $count = drush_idu_fix_OPERATION($pid, $findReplace=NULL, $dsid=NULL, $xpath=NULL);
 *
 * @param string $first
 *   First PID to process.
 * @param string $fix
 *   The Fix operation to be performed.  This string completes the name of an existing idu_drush_fix_* function.
 */
function drush_idu_iduFix($first, $fix) {

  // Get the fix function parameter...
  $function = 'idu_drush_fix_'.$fix;
  if (!function_exists($function)) { drush_log("The specified Fix function '$function' does not exist!", 'error'); }

  if (!$pids = idu_drush_prep($first, $last, $pidn, $range, $ns, TRUE)) { return; }     // Get the PIDs and initialize the loop...

  // Pre-process some options...
  // Check for a --substitutionsFile option, instead of $find and $replace, and process accordingly.
  $findReplace = NULL;
  $file = drush_get_option('substitutionsFile', NULL);
  if (!is_null($file)) {

    // CSV file processing
    if (!$f = fopen($file,"r")) { die("Unable to parse findReplace INI file '$file' for reading!  "); }
    while (!feof($f)) {
      list($find,$replace) = fgetcsv($f);
      $findReplace[trim($find)] = trim($replace);
    }
    fclose($f);

    /* INI file processing
    if (!$findReplace = parse_ini_file($file)) { die("Unable to parse findReplace INI file '$file' for reading!  "); } */

    $msg = count($findReplace)." substitution pair(s) found in '$file'.";
    drush_log($msg, 'status');

  } else {
    $find = drush_get_option('find', NULL);
    $replace = drush_get_option('replace', NULL);
    $findReplace[$find] = $replace;
  }

  // Check for --find and --dsid options.
  if ($find = drush_get_option('find', NULL)) { $preOpts['find'] = $find; }
  if ($dsid = drush_get_option('dsid', NULL)) { $preOpts['dsid'] = $dsid; }

  // Put the pre-fetched options into an array and pass it to the hook function.
  $preOpts['findReplace'] = $findReplace;

  idu_drush_print_progress("iduFix - $fix");
  $count = 0;

  // If there's only one PID... process it.
  if (count($pids) === 1) {
    $count += call_user_func($function, $pids[0], $preOpts);
    idu_drush_post_to_file("iduFix - $fix: $pids[0] complete.", NULL, 'public://iduFix_progress.log');
    idu_drush_print_progress("iduFix - $fix", 0, 1);

  } else {   // Operate on a range of PIDs.
    for ($i=0, $p=$pidn; $i<$range; $i++, $p++) {
      $pid = "$ns:$p";
      if (in_array($p, $pids)) {
        $count += call_user_func($function, $pid, $preOpts);
      }
      idu_drush_post_to_file("iduFix - $fix: $pid complete.", NULL, 'public://iduFix_progress.log');
      idu_drush_print_progress("iduFix - $fix", $i, $range);
    }
  }

  // Done.
  $finish = date('H:i:s');
  drush_log("Completed $count 'iduFix - $fix' operations at $finish. \n", 'status');

}

/**
 * Drush idu_special_* function to fix object titles and local IDs.
 *
 * Invoke this function via iduS and the 'FixLocal' operation parameter.
 * Something like this...
 *   drush iduS faulconer-art:1-501 FixLocal
 *
 * @param int $pid
 *   The PID number(s) of the object(s) to operate on.
 * @return int
 *   The number of successful operations performed, or zero if none.
 */
function idu_drush_special_FixLocal($pid) {
  $local = "/mods:mods/mods:identifier[@type='local']";
  $title = "/mods:mods/mods:titleInfo[not(@type)]/mods:title";

  $total = $count = 0;

  list($ns, $pidRange) = explode(':', $pid);
  list($p1, $p2) = explode("-", $pidRange);
  if (!$p2) { $p2 = $p1; }

  for ($i=$p1; $i<=$p2; $i++) {
    $count = 0;
    $pid = "$ns:$i";
    try {
      $object = idu_repo()->getObject($pid);
    } catch (Exception $e) {
      drush_log("No object found at $pid!", 'error');
      continue;     // for any exception...skip this PID
    }

    if ($ds = $object['MODS']) {

      // Pull the local ID, if any, from the object title and make it the new
      // local ID element in MODS.

      if ($label = $object->label) {
        if (strpos($label, ']') > 0) {
          list($ID, $remainder) = explode(']', $label);
          $id = trim($ID, '[ ');
          if (strlen($id) > 1) {
            $newTitle = trim($remainder);
            if (strlen($newTitle) < 1) { $newTitle = "Untitled"; }
            $object->label = $newTitle;
            $content = $ds->content;
            $xml = new DOMDocument;
            $xml->loadXML($content);
            $dx = new DOMXPath($xml);
            $results = $dx->query($title);
            foreach ($results as $node) {
              $count++;
              $node->nodeValue = $newTitle;
              $content = $xml->saveXML();
              break;
            }
            $xml->loadXML($content);
            $dx = new DOMXPath($xml);
            $results = $dx->query($local);
            foreach ($results as $node) {
              $count++;
              $node->nodeValue = $id;
              $content = $xml->saveXML();
              break;
            }
          }
        }
      }
    }

    if ($count > 0) {
      $total += $count;
      $object['MODS']->setContentFromString($content);
    }
  }

  return $total;
}

/**
 * Drush idu_special_* function to fix object titles.
 *
 * Invoke this function via iduS and the 'FixObjectTitle' operation parameter.  Something like this...
 *   drush iduS faulconer-art:2000-2500 FixObjectTitle
 *
 * @param int $pid
 *   The PID number(s) of the object(s) to operate on.
 * @return int
 *   The number of successful operations performed, or zero if none.
 */
function idu_drush_special_FixObjectTitle($pid) {
  $xpath = "/mods:mods/mods:titleInfo[not(@type)]/mods:title";
  $total = $count = 0;

  list($ns, $pidRange) = explode(':', $pid);
  list($p1, $p2) = explode("-", $pidRange);
  if (!$p2) { $p2 = $p1; }

  for ($count=0, $i=$p1; $i<=$p2; $i++) {
    $pid = "$ns:$i";
    try {
      $object = idu_repo()->getObject($pid);
    } catch (Exception $e) {
      drush_log("No object found at $pid!", 'error');
      continue;     // for any exception...skip this PID
    }

    if ($ds = $object['MODS']) {
      $db = or_connect();
      $table = or_make_table($db, $ns);
      $sql = "SELECT ?n FROM ?n WHERE ?n=?s";
      if ($title = $db->getOne($sql, 'D', $table, 'pidn', $i)) {
        $content = $ds->content;
        $xml = new DOMDocument;
        $xml->loadXML($content);
        $dx = new DOMXPath($xml);
        if ($results = $dx->query($xpath)) {
          foreach ($results as $node) {
            $object->label = $title;
            $node->nodeValue = $title;
            $content = $xml->saveXML();
            $object['MODS']->setContentFromString($content);
            break;
          }
        }
      }
    }
  }

return;
}

/**
 * Drush idu_fix_* function to purge a specified datastream (and derivatives).
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF $pids PurgeDatastream --dsid=PURGE --self=public://cleanup_mods.xsl
 *
 * Applicable options are:
 *   --self=<path to the self-transform XSL file>  Defaults to NULL.
 *   --dsid=The existing datastream ID such as 'MODS', 'MADS', 'DC', etc.  Defaults to 'MODS'.
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of repair operations performed, or zero if none.
 */
function idu_drush_fix_PurgeDatastream($pid) {

  try {
    $object = idu_repo( )->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Check that the specified datastream exists... if not, we're done.
  $dsid = drush_get_option('dsid', 'MODS');
  if (!$ds = $object[$dsid]) { return 0; }

  // Call the Fedora API to remove the datastream.
  $api_m = $object->repository->api->m;                       // For a Management API.
  $params = array( );
  $result = $api_m->purgeDatastream($pid, $dsid, $params);

  // If this was an OBJ datastream... remove the TN, JPG and JP2 also!
  if ($dsid === "OBJ") {
    $result = $api_m->purgeDatastream($pid, 'JP2', $params);
    $result = $api_m->purgeDatastream($pid, 'JPG', $params);
    $result = $api_m->purgeDatastream($pid, 'TN', $params);
  }

  // Re-index this object in Solr.
  update_solr_index($pid);

  return 1;
}

/**
 * Drush idu_fix_* function to change an existing object to a parent and make
 * the content and metadata into a new child of the parent.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF $pids MakeFamily
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of children added, or zero if none.
 */
function idu_drush_fix_MakeFamily($pid) {
  // @TODO...all of this!
  // Fetch the intended parent object...

  try {
    $object = idu_repo()->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Fetch the existing sibling object...

  if (!$siblingPID = drush_get_option('sibling', NULL)) {
    drush_log('No existing sibling PID specified!', 'error');
    return 0;
  }

  try {
    $sibling = idu_repo()->getObject($siblingPID);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  $data = islandora_compound_object_retrieve_compound_info($sibling);

  // Determine the parent PID and sequence number for the new addition...

  $parentPID = $data['parent_pid'];
  $num = $data['child_count'];
  $next = $num + 1;

  // If we have a legitimate parent with at least one child, open the object's
  // RELS-EXT and attach it to the parent.

  if ($parentPID && $num > 0) {
    $rx = $object['RELS-EXT'];
    $xml = new DOMDocument;
    $xml->loadXML($rx->content);
    $dx = new DOMXPath($xml);

    // Find the rdf:RDF/rdf:Description...the parent of our new elements.

    $parents = $dx->query("/rdf:RDF/rdf:Description");
    $element1 = $xml->createElement("fedora:isConstituentOf");
    $attr = $xml->createAttribute("rdf:resource");
    $attr->value = "info:fedora/" . $parentPID;
    $element1->appendChild($attr);
    $parent_PID = str_replace(":", "_", $parentPID);
    $element2 = $xml->createElement("islandora:isSequenceNumberOf" . $parent_PID, $next);

    foreach ($parents as $parent) {
      $parent->appendChild($element1);
      $parent->appendChild($element2);
      $content = $xml->saveXML();
      $object['RELS-EXT']->setContentFromString($content);
      return 1;
      break;
    }
    return 0;
  }
  return 0;
}

/**
 * Drush idu_fix_* function to ensure dc:date, dc:creator and dc:source are present.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF $pids UpdateDC --date=Today --creator='Mark McFate'
 *
 * Applicable options are:
 *   --date=string to populate dc:date value.  Defaults to 'Unspecified'.
 *   --creator=string to populate dc:creator value.  Defaults to 'Unknown'.
 *   --source=DC field to build dc:source from.  Defaults to 'dc:creator'.
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of children added, or zero if none.
 */
function idu_drush_fix_UpdateDC($pid) {

  // Fetch the intended object...

  try {
    $object = idu_repo()->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Open the DC datastream and have a look for dc:date and dc:creator.

  if (!$dc = $object['DC']) { return 0; }
  $total = 0;

  $xml = new DOMDocument;
  $xml->loadXML($dc->content);
  $dx = new DOMXPath($xml);

  // Find the oai_dc:dc...the parent of our target elements.

  $parents = $dx->query("/oai_dc:dc");

  foreach ($parents as $parent) {         // There will be only ONE!

    // Now look for dc:date... if not there append a new element to $parent.

    $target = $dx->query("/oai_dc:dc/dc:date");

    if ($target->length === 0) {
      $val = drush_get_option('date', 'Unspecified');
      $new = $xml->createElement("dc:date", $val);
      $parent->appendChild($new);
      $content = $xml->saveXML();
      $total += 1;
      $object['DC']->setContentFromString($content);
    }

    // Now look for dc:creator... if not there append a new element to $parent.

    $target = $dx->query("/oai_dc:dc/dc:creator");

    if ($target->length === 0) {
      $val = drush_get_option('creator', 'Unknown');
      $new = $xml->createElement("dc:creator", $val);
      $parent->appendChild($new);
      $content = $xml->saveXML();
      $total += 1;
      $object['DC']->setContentFromString($content);
    }

    // Now look for dc:source... if not there, use the --source option (defaults
    // to dc:creator) to build a new dc:source.

    $target = $dx->query("/oai_dc:dc/dc:source");

    if ($target->length === 0) {
      $field = drush_get_option('source', 'dc:creator');
      $parts = $dx->query("/oai_dc:dc/$field");
      $val = "";
      foreach ($parts as $part) {
        $val .= $part->nodeValue."; ";
      }
      $value = trim($val,'; ');
      $new = $xml->createElement("dc:source", $value);
      $parent->appendChild($new);
      $content = $xml->saveXML();
      $total += 1;
      $object['DC']->setContentFromString($content);
    }

  }

  return $total;
}


/**
 * Drush idu_fix_* function to purge all --xpath matched metadata elements from
 * the datastream identified by --dsid.
 *
 * Invoke this function with an iduF process using a command of the form:
 *   drush iduF <pids> PurgeElements --dsid=DC --xpath="/oai_dc:dc/dc:description"
 *
 * Applicable options are:
 *   --dsid=the ID of the datastream to be searched
 *   --xpath=the Xpath of the element(s) to be purged from --dsid
 *
 * @param string $pid
 *   The PID (from the list of first-to-last) to operate on.
 * @return int
 *   The number of operations completed, or zero if none.
 */
function idu_drush_fix_PurgeElements($pid) {

  // Fetch the intended object...

  try {
    $object = idu_repo()->getObject($pid);
  } catch (Exception $e) {
    return 0;  // for any exception...skip this PID
  }

  // Open the --dsid datastream...

  if (!$dsid = drush_get_option('dsid',FALSE)) { return 0; }
  if (!$dc = $object[$dsid]) { return 0; }
  $total = 0;

  $xml = new DOMDocument;
  $xml->loadXML($dc->content);
  $dx = new DOMXPath($xml);

  // Find the target elements.

  if (!$xpath = drush_get_option('xpath',FALSE)) { return 0; }
  $elements = $dx->query($xpath);

  // Mark them for removal...note that you can't do this in one pass as $elements
  // gets corrupted as bits are removed!

  $toRemove = array( );
  foreach ($elements as $element) {
    $toRemove[ ] = $element;
  }

  // Do it.

  foreach( $toRemove as $element ){
    $element->parentNode->removeChild($element);
    $content = $xml->saveXML();
    $total += 1;
  }

  $object['DC']->setContentFromString($content);
  return $total;
}