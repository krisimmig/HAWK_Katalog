<?php
defined("DS") ? null : define("DS", DIRECTORY_SEPARATOR);

//defined("SITE_ROOT") ? null : define("SITE_ROOT",DS."wamp".DS."www".DS."blog");
//defined("SITE_ROOT") ? null : define("SITE_ROOT", DS."customers".DS."k-im.de".DS."k-im.de".DS."httpd.www".DS."blog");
defined("SITE_ROOT") ? null : define("SITE_ROOT", DS."customers".DS."c".DS."1".DS."6".DS."k-im.de".DS."httpd.www".DS."blog");
defined("LIB_PATH") ? null : define("LIB_PATH", SITE_ROOT.DS."includes");
defined('IMG_UPLOAD_PATH') ? null : define("IMG_UPLOAD_PATH", SITE_ROOT.DS."public".DS."images");
defined('F_IMG_UPLOAD_PATH') ? null : define("F_IMG_UPLOAD_PATH", SITE_ROOT.DS."public".DS."featured_images");
defined('IMG_PATH') ? null : define("IMG_PATH", "images");
defined('F_IMG_PATH') ? null : define("F_IMG_PATH", "featured_images");

require_once(LIB_PATH.DS."config.php");
require_once(LIB_PATH.DS."functions.php");

require_once(LIB_PATH.DS."logger.php");
require_once(LIB_PATH.DS."database.php");
require_once(LIB_PATH.DS."database_object.php");
require_once(LIB_PATH.DS."session.php");
require_once(LIB_PATH.DS."pager.php");

require_once(LIB_PATH.DS."post.php");
require_once(LIB_PATH.DS."user.php");
require_once(LIB_PATH.DS."photograph.php");
require_once(LIB_PATH.DS."comments.php");

?>