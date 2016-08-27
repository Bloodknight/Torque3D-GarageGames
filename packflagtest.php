<?php
//--------------------------------------------------------------------------------------
// Static Network BitStream Analyzer for Torque 3D (c) 2012 Bitgap Games by Konrad Kiss
//--------------------------------------------------------------------------------------
 
$sourcedir = "G:/Torque3D/engine/source/";
 
error_reporting(E_ALL);
function errorhandler($errno, $errstr, $errfile, $errline) {
   echo($errstr." in ".$errfile." on line ".$errline); 
   exit;
//   return true;
}
$old_error_handler = set_error_handler("errorhandler");
 
$cmdCosts = array( 
   // write fn => array(regexp, costParamPos (1..) or 0, defaultCost)
   "writeInt" => array("stream\-\>writeInt[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 32),
   "readInt" => array("stream\-\>readInt[\W]*\(([^)\n]+)\)", 1, 32),
   "mathWrite" => array("mathWrite[\W]*\([\W]*\*[\W]*stream[\W]*\,([^)\n]+)\)", 0, 64),
   "mathRead" => array("mathRead[\W]*\([\W]*\*[\W]*stream[\W]*\,([^)\n]+)\)", 0, 64),
   "writeCussedU32" => array("stream\-\>writeCussedU32[\W]*\(([^)\n]+)\)", 0, 37),
   "readCussedU32" => array("stream\-\>readCussedU32[\W]*\(([^)\n]*)\)", 0, 37),
   "writeSignedInt" => array("stream\-\>writeSignedInt[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 32),
   "readSignedInt" => array("stream\-\>readSignedInt[\W]*\(([^)\n]+)\)", 1, 32),
   "writeRangedU32" => array("stream\-\>writeRangedU32[\W]*\(([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "readRangedU32" => array("stream\-\>readRangedU32[\W]*\(([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "writeRangedS32" => array("stream\-\>writeRangedS32[\W]*\(([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "readRangedS32" => array("stream\-\>readRangedS32[\W]*\(([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "writeFloat" => array("stream\-\>writeFloat[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 32),
   "readFloat" => array("stream\-\>readFloat[\W]*\(([^)\n]+)\)", 1, 32),
   "writeSignedFloat" => array("stream\-\>writeSignedFloat[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 32),
   "readSignedFloat" => array("stream\-\>readSignedFloat[\W]*\(([^)\n]+)\)", 1, 32),
   "writeRangedF32" => array("stream\-\>writeRangedF32[\W]*\(([^,\n]+)\,([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 4, 32),
   "readRangedF32" => array("stream\-\>readRangedF32[\W]*\(([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 3, 32),
   "writeClassId" => array("stream\-\>writeClassId[\W]*\(([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "readClassId" => array("stream\-\>readClassId[\W]*\(([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "writeNormalVector" => array("stream\-\>writeNormalVector[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 96),
   "readNormalVector" => array("stream\-\>readNormalVector[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 96),
   "writeCompressedPoint" => array("stream\-\>writeCompressedPoint[\W]*\(([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "readCompressedPoint" => array("stream\-\>readCompressedPoint[\W]*\(([^,\n]+)\,([^)\n]+)\)", 0, 32),
   "writeVector" => array("stream\-\>writeVector[\W]*\(([^,\n]+)\,([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 4, 32),
   "readVector" => array("stream\-\>readVector[\W]*\(([^,\n]+)\,([^,\n]+)\,([^,\n]+)\,([^)\n]+)\)", 4, 32),
   "writeAffineTransform" => array("stream\-\>writeAffineTransform[\W]*\(([^)\n]+)\)", 0, 96),
   "readAffineTransform" => array("stream\-\>readAffineTransform[\W]*\(([^)\n]+)\)", 0, 96),
   "writeQuat" => array("stream\-\>writeQuat[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 9),
   "readQuat" => array("stream\-\>readQuat[\W]*\(([^,\n]+)\,([^)\n]+)\)", 2, 9),
   "writeQuat2" => array("stream\-\>writeQuat[\W]*\(([^,\n]+)\)", 0, 9),
   "readQuat2" => array("stream\-\>readQuat[\W]*\(([^,\n]+)\)", 0, 9),
   "writeBits" => array("stream\-\>writeBits[\W]*\(([^,\n]+)\,([^)\n]+)\)", 1, 1),
   "readBits" => array("stream\-\>readBits[\W]*\(([^,\n]+)\,([^)\n]+)\)", 1, 1),
   "writeBits2" => array("stream\-\>writeBits[\W]*\(([^)\n]+)\)", 0, 1),
   "readBits2" => array("stream\-\>readBits[\W]*\(([^)\n]+)\)", 0, 1),
//   "writeFlagTrue" => array("stream\-\>writeFlag[\W]*\([ ]*true[ ]*\)", 0, 0), // trick to not have to parse conditions
//   "writeFlagFalse" => array("stream\-\>writeFlag[\W]*\([ ]*false[ ]*\)", 0, 0), // trick to not have to parse conditions
   "writeFlag" => array("stream\-\>writeFlag[\W]*\(([^)\n]+)\)", 0, 1),
   "readFlag" => array("stream\-\>readFlag[\W]*\(([^)\n]*)\)", 0, 1),
   "readFlag2" => array("_readDirtyFlag[\W]*\([\W]*stream[\W]*\,([^)\n]+)\)", 0, 1), // sfxEmitter exotics
   "writeString" => array("stream\-\>writeString[\W]*\(([^,\n]+)\,([^)\n]+)\)", 1, 256*8),
   "writeString2" => array("stream\-\>writeString[\W]*\(([^)\n]+)\)", 0, 256*8),
   "readString" => array("stream\-\>readString[\W]*\(([^)\n]+)\)", 0, 256*8),
   "readSTString" => array("stream\-\>readSTString[\W]*\(([^)\n]*)\)", 0, 256*8),
   "write" => array("stream\-\>write[\W]*\(([^)\n]+)\)", 0, 64),
   "read" => array("stream\-\>read[\W]*\(([^)\n]+)\)", 0, 64),
);
 
$errorcount = 0;
$currentFile = "";
$output = "";
$summary = "";
 
uecho("// ****************************************************************\n");
find_files($sourcedir, "/cpp$/", "examine");
file_put_contents($sourcedir."unbalanced.txt", "// SUMMARY\n\n\n".$summary."\n\n\n\n// DETAILED OUTPUT".$output);
uecho("// ****************************************************************\n");
 
//------------------------------------------------------------------------------
function fn_countbits($source, $bitstreamName)
//------------------------------------------------------------------------------
{
   global $cmdCosts, $currentFile;
   $lines = explode("\n", $source);
   $count = 0;
   $cmdlist = "";
   for ($l=0;$l<count($lines);$l++) {
      //uecho(".");
      $line = $lines[$l];
      // strip comments
      foreach ($cmdCosts as $cmd => $data) {
         $pattern = "/".str_replace("stream", $bitstreamName, $data[0])."/s";
         preg_match($pattern, $line, $params);
         if (count($params)>0)
         {
            if ($data[1]>0) {
               // there is a parameter that explicitly defines the number of used bits for this operation
               if (is_numeric($params[$data[1]])) {
                  // the cost is a clear numeric bit count
                  $cost = $params[$data[1]];
               } else {
                  // the cost could be a variable or a function perhaps - we'll default to a cost of 1 here
                  $cost = 1;
               }
            } else {
               // no paremeters are used to define bitcount... we can then use a default bitcount for comparison
               // it could be 0, which means that the command should not be used to compare values
               // It is used in the often case when writing a flag is broken up into the two branches of a condition while
               // only one command is reading the other side.
               $cost = $data[2];
            }
            //uecho($cmd." : ".$cost."\n");//." [".$data[1]." / ".$params[$data[1]]." / ".$data[2]."]\n");
            $count += $cost;
            $cmdlist .= $cmd." - ".$cost." @ line ".$l."\n";
            if ($cmd=="writeFlagTrue" || $cmd=="writeFlagFalse")
               break;
         }
      }
       
      //uecho(trim($lines[$l]," \t\n\r")."\n");
   }
   //$cmdcount = substr_count($source, $bitstreamName."->");
   return array($count, $cmdlist);
}
 
//------------------------------------------------------------------------------
function compare($filepath, $fn1, $fn2)
//------------------------------------------------------------------------------
{
   global $errorcount;
 
   $content = file_get_contents($filepath);
 
   // remove comments (basic, but mostly does the job)
   $content = preg_replace('/(\/\/[^\n]*\n)/sm', "\n", $content);
   $content = preg_replace('/(\/\*.*?\*\/)/sm', "", $content);
 
   // get rid of all return characters
   $content = str_replace("\r", "", $content);
 
   // replace multiple new lines with a single new line
   $content = preg_replace('/([\n]+)/sm', "\n", $content);
 
   // remove irrelevant lines
   $wasif = true;
   $wassec = true;
   $wasfn = true;
   $wasrw = true;
   $wassd = false;
   $lvl = 0;
   $modified = true;
   $newcontent = "";
   foreach (explode("\n", $content) as $linenum => $line)
   {
      //uecho("*");
      // replace multiple tabs and spaces with single spaces
      //$line = preg_replace('/([\t ]+)/sm', " ", $line);
      //$line = trim($line, " \t");
      $line = rtrim($line, " \t");
 
      // remove strings between ""
      //$line = preg_replace('/(\")[^\"]*(\")/', "", $line);
 
      if ($line=="") {
         continue;
      }
 
      $keep = false;
 
      $if0 = (strpos($line, "if (") !== false ||
               strpos($line, "if(") !== false );
      $if1 = (strpos($line, "else") !== false );
      $if = ($if0 || $if1);
 
      $sec =(strpos($line, "{") !== false ||
               strpos($line, "}") !== false );
      $fn = (strpos($line, "::".$fn1) !== false ||
               strpos($line, "::".$fn2) !== false );
      $rw = (strpos($line, "->write") !== false ||
               strpos($line, "->read") !== false ||
               strpos($line, "mathRead") !== false ||
               strpos($line, "mathWrite") !== false );
 
      $sd = strpos($line, ";") !== false;
      $sp = strpos($line, "(") !== false;
      $ep = strpos($line, ")") !== false;
 
      $separator = "\n";
 
      if (!$wassd && !$wassec && !$if && !$rw) {
         $separator = " ";
         $line = trim($line, " \t");
         $modified = true;
      }
 
      $newcontent .= $separator.$line;
 
      $wasif = $if;
      $wassec = $sec;
      $wasfn = $fn;
      $wasrw = $rw;
      $wassd = $sd;
   }
   $content = $newcontent;
 
   // remove empty {}-s
   //$content = preg_replace('/(\{[ \t\n\r]*\})/sm', "", $content);
 
   $fnone = fn_contents($content, $fn1);
   $fntwo = fn_contents($content, $fn2);
 
   if ($fnone === false && $fntwo == false)
      return;
 
   if (($fnone === false || $fntwo === false)) {
      $errorcount++;
      uecho("\n\n\n");
      uecho("// ".$errorcount." Missing either a(n) ".$fn1." or a(n) ".$fn2." method in ".$filepath.".\n", true);
      return;
   }
 
   $one = fn_countbits($fnone[1], $fnone[0]);
   $two = fn_countbits($fntwo[1], $fntwo[0]);
 
   if ($one[0] != $two[0]) {
      $errorcount++;
      uecho("\n\n\n");
      uecho("// ".$errorcount." Bitcount mismatch between ".$fn1." [".$one[0]."] and ".$fn2." [".$two[0]."] in ".$filepath.".\n", true);
      uecho("\n// ".str_pad($fn1." (".$fnone[0]." - ".$one[0].") ", 66, "+")."\n");
      uecho($fnone[1]);
      uecho("\n\n".$one[1]);
      uecho("\n// ".str_pad($fn2." (".$fntwo[0]." - ".$two[0].") ", 66, "+")."\n");
      uecho($two[1]."\n\n");
      uecho($fntwo[1]);
      uecho("\n// ".str_pad("", 66, "+")."\n");
   }
}
 
//------------------------------------------------------------------------------
function examine($filepath)
//------------------------------------------------------------------------------
{
   global $currentFile;
   $currentFile = $filepath;
   compare($filepath, "pack", "unpack");
   compare($filepath, "packData", "unpackData");
   compare($filepath, "packUpdate", "unpackUpdate");
   compare($filepath, "readPacketData", "writePacketData");
}
 
//------------------------------------------------------------------------------
function fn_contents($content, $functionName)
//------------------------------------------------------------------------------
{
   $fnofs = strpos($content, "::".$functionName);
   if ($fnofs === false)
      return;
 
   // find the last enter before the function
   $lastenterpos = strrpos(substr($content, 0, $fnofs), "\n");
   $content = substr($content, $lastenterpos+1);
   $content = str_replace("\"@", "[at]", $content);   // we need the @ and ` characters to replace } and {
                                                      // the at sign is only found in doc references
   $end = 0;
   $lvl = 0;
   $l = strlen($content);
   for ($p=0;$p<$l;$p++) {
      $ch = substr($content, $p, 1);
      if ($ch=="{") {
         if ($lvl>0)
            $content = substr_replace($content, "`", $p, 1);
         $lvl++;
      } else if ($ch=="}") {
         if ($lvl>1)
            $content = substr_replace($content, "@", $p, 1);
         if ($lvl>0) {
            $lvl--;
            if ($lvl==0) {
               $end = $p;
               break;
            }
         }
      }
   }
   $content = substr($content, 0, $end+1);
 
   $pattern = "/(\w+) \b(\w+)::".$functionName."\(([^{]+)\{(.+?)}/s";
   preg_match($pattern, $content, $matches);
   if (count($matches)>0)
   {
      $bitstreamName = trim(substr($matches[3], strpos($matches[3], "BitStream")+9), " *)\n\t\r");
      $par = strpos($bitstreamName, ")");
      if ($par) $bitstreamName = substr($bitstreamName, 0, $par-1);
      $com = strpos($bitstreamName, ",");
      if ($com) $bitstreamName = substr($bitstreamName, 0, $com-1);
 
      $fnsource = str_replace("`", "{", str_replace("@", "}", $matches[4]));
      $fnsource = str_replace("[at]", "@", $fnsource);
   } else {
      return false;
   }
 
   $res = array();
   $res[0] = $bitstreamName;
   $res[1] = $fnsource;
 
   return $res;
}
 
// http://snippets.dzone.com/posts/show/4147
//------------------------------------------------------------------------------
function find_files($path, $pattern, $callback)
//------------------------------------------------------------------------------
{
   $path = rtrim(str_replace("\\", "/", $path), '/') . '/*';
   foreach (glob ($path) as $fullname) {
      //uecho("%");
      if (basename($fullname)=="_svn" || basename($fullname)==".svn")
         continue;
      if (is_dir($fullname)) {
         //uecho("\n".$fullname.":\n", true);
         find_files($fullname, $pattern, $callback);
      } else if (preg_match($pattern, $fullname)) {
         //uecho("        ".$fullname." (" . round(filesize($fullname)/1024, 2) . "kBytes\n");
         call_user_func($callback, $fullname);
      }
   }
}
 
//------------------------------------------------------------------------------
function uecho($str, $important=false)
//------------------------------------------------------------------------------
{
   global $output, $summary;
   if ($important==true) {
      $summary .= $str;
   }
   $output .= $str;
 echo($str);
}
 
?>