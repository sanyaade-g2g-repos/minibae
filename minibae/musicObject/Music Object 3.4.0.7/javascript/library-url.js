/**********
     TITLE: URL Library
   VERSION: 2.0.2
      TYPE: Code Library
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/
function URL_a(){var _a='',_b=false,_c= -1;while(!_b){var _d=this.indexOf('\\',_c+1);if(_d== -1){_d=this.length;_b=true;}_a+=this.substring(_c+1,_d)+(!_b?'/':'');if(_d==this.length-1)_b=true;_c=_d;}return _a;}function URL_b(_e){if(typeof _e!='string')_e=baseURL;_e=_e.toForwardSlashes();var _f=this.toForwardSlashes(),_a='',_g=_e.substring(0,_e.lastIndexOf('/')+1),_h=false;while(!_h){if(_f.substring(0,_g.length)==_g){_h=true;}else{_d=_g.lastIndexOf('/',_g.length-2);if(_d== -1)_h=true;_a+='../';_g=_g.substring(0,_d+1);}}_a+=_f.substring(_g.length);return _a;}function URL_c(_e){if(typeof _e!='string')_e=baseURL;_e=_e.toForwardSlashes();var _f=this.toForwardSlashes();if(_f.indexOf(':')!= -1&&(_f.indexOf(':')<_f.indexOf('/')||_f.indexOf('/')== -1)){return _f;}else{var _a=_e.substring(0,_e.lastIndexOf('/')),_b=false,_c= -1,_i;
while(!_b){var _d=_f.indexOf('/',_c+1);if(_d== -1){_d=_f.length;_b=true;}var _j=_f.substring(_c+1,_d);if(_j=='..'){_i=_a.lastIndexOf('/');_a=(_i== -1)?'':_a.substring(0,_i);}else{if(_a!='')_a+='/';_a+=_j;}_c=_d;}return _a;}}function URL_d(_k,_l,_m){var _n='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890',_o='.-_~@#&+',_a='',_p=false,_q=false,_r=false,_s=this;if(typeof _l!='string'||_o.indexOf(_l)== -1)_l='-';if(typeof _m!='string'||_o.indexOf(_m)== -1)_m='';if(typeof _k!='string')_k='lower';if(_k=='upper'){_s=_s.toUpperCase();}else if(_k=='lower'){_s=_s.toLowerCase();}for(var _t=0;_t<_s.length;_t++){var _u=_s.charAt(_t);if(_u==' '||_o.indexOf(_u)!= -1){if(_u==' '){_p=false;}else{if(!_p)_q=true;_a+=_u;}}else{if(!_p){_p=true;if(_r&& !_q)_a+=_l;_q=false;}_a+=(_n.indexOf(_u)!= -1)?_u:_m;if(!_r)_r=true;}}return _a;}String.prototype.toForwardSlashes=URL_a;String.prototype.toRelativeURL=URL_b;String.prototype.toAbsoluteURL=URL_c;String.prototype.urlize=URL_d;
baseURL=window.location.href.toForwardSlashes();baseURL=baseURL.substring(0,baseURL.lastIndexOf('/')+1);