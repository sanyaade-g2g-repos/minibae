/**********
     TITLE: Music Object Extension - Flash
   VERSION: 1.2.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/
function mo_doFSCommand(_a,_b){eval(_a+((_a.indexOf('(')== -1)?('('+_b+')'):''));}function mo_wireFlashMovie(_c){window[_c+'_DoFSCommand']=mo_doFSCommand;if(Music.playerType=='ActiveX')document.writeln('<SCRIPT LANGUAGE="VBScript">'+'Sub '+_c+'_FSCommand (ByVal command, ByVal args)'+'call mo_doFSCommand (command, args)'+'end sub'+'</SCRIPT>');}function mo_embedFlashMovie(_d){var _e=Music.MO_x8_a,_f=new Array('SRC','WIDTH','HEIGHT','ALIGN','HSPACE','VSPACE','SALIGN','PLAY','LOOP','QUALITY','SCALE','MENU','BGCOLOR','BASE');for(var _g=0;_g<_f.length;_g++)_e[_f[_g]]=null;_e.HSPACE=_e.VSPACE='0';Music.parseAttributes(_d,_e);var _h='';if(Music.playerType=='Plug-in'){
_h='<EMBED TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"'+Music.tagAttr(_e,'NAME')+' SWLIVECONNECT='+(_e.NAME!=null)+' MAYSCRIPT';for(var _g=0;_g<_f.length;_g++)_h+=Music.tagAttr(_e,_f[_g]);_h+='>';}else if(Music.playerType=='ActiveX'){_h+='<OBJECT'+((_e.NAME!=null)?(' ID="'+_e.NAME+'"'):'')+' WIDTH='+_e.WIDTH+' HEIGHT='+_e.HEIGHT+Music.tagAttr(_e,'ALIGN')+Music.tagAttr(_e,'HSPACE')+Music.tagAttr(_e,'VSPACE')+' CLASSID="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" CODEBASE="http://active.macromedia.com/flash/cabs/swflash.cab">\n'+'<PARAM NAME="movie" VALUE="'+_e.SRC+'">\n';for(var _g=0;_g<_f.length;_g++){if(_e[_f[_g]]!=null)_h+='<PARAM NAME="'+_f[_g]+'" VALUE="'+_e[_f[_g]]+'">\n';}_h+='</OBJECT>';}document.writeln(_h);if(_e.NAME!=null)mo_wireFlashMovie(_e.NAME);}Music.MO_x8_a=new Array();Music.addPublicStaticMethods('embedFlashMovie','wireFlashMovie');