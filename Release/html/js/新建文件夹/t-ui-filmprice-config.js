var UIFilmPriceConfig={createNew:function(){function h(){$.ajax({type:"POST",url:"film-getprice.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(a){c.val(a.price)},error:function(){toastr.error("\u83b7\u53d6\u8986\u819c\u5355\u4ef7\u5931\u8d25,\u8bf7\u5237\u65b0\u91cd\u8bd5\uff01")}})}function n(){k.on("click",function(){if(isNull(c.val().trim()))toastr.error("\u8986\u819c\u5355\u4ef7\u4e0d\u80fd\u4e3a\u7a7a\uff01");else if(isPrice(c.val().trim())){var a={price:parseFloat(c.val().trim())};
$.ajax({type:"POST",url:"film-setprice.zc",cache:!1,data:JSON.stringify(a),dataType:"json",success:function(a){toastr.success("\u65b0\u8986\u819c\u5355\u4ef7\u8bbe\u7f6e\u6210\u529f");h()},error:function(){toastr.error("\u65b0\u8986\u819c\u5355\u4ef7\u8bbe\u7f6e\u5931\u8d25,\u8bf7\u5237\u65b0\u91cd\u8bd5\uff01")}})}else toastr.error("\u8986\u819c\u5355\u4ef7\u8f93\u5165\u4e0d\u5408\u6cd5\uff0c\u4ec5\u80fd\u7cbe\u786e\u5230\u5c0f\u6570\u70b9\u540e\u4e24\u4f4d\uff01")})}function g(){f.dataTable({destroy:!0,
ajax:function(a,p,c){$.ajax({type:"POST",url:"film-morerule-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(c){var b={};b.draw=a.draw;b.recordsTotal=c.recordsTotal;b.recordsFiltered=c.recordsFiltered;b.data=c.data;p(b)}})},columns:q})}function r(){l.on("click",function(){var a;isNull(b.val().trim())?toastr.error("\u5f00\u59cb\u533a\u95f4\u4e0d\u80fd\u4e3a\u7a7a\uff01"):isInt(b.val().trim())?isNull(d.val().trim())?toastr.error("\u7ed3\u675f\u533a\u95f4\u4e0d\u80fd\u4e3a\u7a7a\uff01"):
isInt(d.val().trim())?isNull(e.val().trim())?toastr.error("\u51fa\u6570\u4e0d\u80fd\u4e3a\u7a7a\uff01"):isInt(e.val().trim())?(a={begin:parseInt(b.val().trim()),end:parseInt(d.val().trim()),count:parseInt(e.val().trim())},$.ajax({type:"POST",url:"film-morerule-add.zc",contentType:"application/json;charset=utf-8",data:JSON.stringify(a),dataType:"json",success:function(a){0==a.ok?(toastr.success("\u51fa\u6570\u89c4\u5219\u6dfb\u52a0\u6210\u529f"),g(),b.val(""),d.val(""),e.val("")):toastr.error("\u51fa\u6570\u89c4\u5219\u6dfb\u52a0\u5931\u8d25,"+
a.errorinfo)},error:function(){toastr.error("\u51fa\u6570\u89c4\u5219\u6dfb\u52a0\u5931\u8d25,\u8bf7\u68c0\u67e5\u670d\u52a1\u5668\u53ca\u7f51\u7edc\u540e\u91cd\u8bd5\uff01")}})):toastr.error("\u51fa\u6570\u4ec5\u80fd\u8f93\u5165\u6570\u5b57\uff01"):toastr.error("\u7ed3\u675f\u533a\u95f4\u4ec5\u80fd\u8f93\u5165\u6570\u5b57\uff01"):toastr.error("\u5f00\u59cb\u533a\u95f4\u4ec5\u80fd\u8f93\u5165\u6570\u5b57\uff01")})}function t(){f.on("click","a#delrow",function(a){a.preventDefault;var b=f.DataTable().row($(this).parents("tr")).data();
swal({title:"\u63d0\u793a",text:"\u786e\u5b9a\u8981\u5220\u9664\u533a\u95f4["+b.range+"]\u5417?",type:"warning",showCancelButton:!0,confirmButtonColor:"#DD6B55",confirmButtonText:"\u786e\u5b9a\u5220\u9664",cancelButtonText:"\u53d6\u6d88"},function(a){a&&$.ajax({url:"film-morerule-del.zc",type:"POST",dataType:"json",data:JSON.stringify({range:b.range}),success:function(a){0==a.ok?(toastr.success("\u533a\u95f4["+b.range+"]\u5220\u9664\u6210\u529f"),g()):toastr.error("\u533a\u95f4["+b.range+"]\u5220\u9664\u5931\u8d25,"+
a.errorinfo)},error:function(){toastr.error("\u51fa\u6570\u89c4\u5219\u5220\u9664\u5931\u8d25,\u8bf7\u68c0\u67e5\u670d\u52a1\u5668\u53ca\u7f51\u7edc\u540e\u91cd\u8bd5\uff01")}})})})}var m={},u=$("#ui_body"),k=void 0,c=void 0,f=void 0,l=void 0,b=void 0,d=void 0,e=void 0,q=[{data:"range"},{data:"count"},{data:null,className:"center",defaultContent:'<a id="delrow" href="#"><i class="fa fa-trash-o"></i>\u5220\u9664</a>'}];m.load=function(){u.html('<div class="col-md-12 col-sm-12 col-xs-12">\r\n                <div class="x_panel">\r\n                    <div class="x_title">\r\n                        <h2>\u5b9a\u4ef7 <small>\u8986\u819c\u5355\u4ef7</small></h2>\r\n                        <div class="clearfix"></div>\r\n                    </div>\r\n                    <div class="x_content">\r\n                        <p class="text-muted font-13 m-b-30">\r\n                        \u529f\u80fd\u63cf\u8ff0\uff1a\u6b64\u5904\u7528\u6765\u8bbe\u7f6e\u8986\u819c\u5355\u4ef7\u8d39\uff08\u5143/\u6bcf\u5e73\u7c73\uff09\u3002\r\n                        </p>\r\n                        <div class="form-horizontal form-label-left">\r\n                            <div class="form-group">\r\n                                <label class="control-label col-md-1 col-sm-1 col-xs-12">\u5355\u4ef7(\u5143)</label>\r\n                                <div class="col-md-11 col-sm-11 col-xs-12">\r\n                                    <input id="input_impress_price" type="text" class="form-control" placeholder="">\r\n                                </div>\r\n                            </div>\r\n                            <div class="ln_solid">\r\n                                <div class="form-group">\r\n                                    <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\r\n                                        <button id="submit_set_price" class="btn btn-primary">\u8bbe\u7f6e</button>\r\n                                    </div>\r\n                                </div>\r\n                            </div>\r\n                        </div>\r\n                    </div>\r\n                </div>\r\n                <div class="x_panel">\r\n                <div class="x_title">\r\n                <h2>\u603b\u89c8 <small>\u8986\u819c\u51fa\u6570\u89c4\u5219</small></h2>\r\n                <div class="clearfix"></div>\r\n                </div>\r\n                <div class="x_content">\r\n                <p class="text-muted font-13 m-b-30">\r\n                    \u529f\u80fd\u63cf\u8ff0\uff1a\u6b64\u5904\u5c55\u793a\u4e86\u76ee\u524d\u6240\u6709\u7684\u8986\u819c\u51fa\u6570\u89c4\u5219\u4fe1\u606f\uff0c\u60a8\u53ef\u901a\u8fc7\u8be5\u9875\u9762\u8fdb\u884c\u4fee\u6539\u4e0e\u5220\u9664\u3002\r\n                </p>\r\n                <table id="table_filmmorerule" class="table table-striped table-bordered">\r\n                    <thead>\r\n                    <tr>\r\n                        <th>\u8303\u56f4</th>\r\n                        <th>\u51fa\u6570(\u4e2a)</th>\r\n                        <th>\u64cd\u4f5c</th>\r\n                    </tr>\r\n                    </thead>\r\n                    <tbody>\r\n                    </tbody>\r\n                </table>\r\n                </div>\r\n            </div>\r\n        </div>\r\n        <div class="col-md-12 col-sm-12 col-xs-12">\r\n            <div class="x_panel">\r\n                <div class="x_title">\r\n                    <h2>\u65b0\u589e <small>\u8986\u819c\u51fa\u6570\u89c4\u5219</small></h2>\r\n                    <div class="clearfix"></div>\r\n                </div>\r\n                <div class="x_content">\r\n                    <p class="text-muted font-13 m-b-30">\r\n                        \u529f\u80fd\u63cf\u8ff0\uff1a\u60a8\u53ef\u901a\u8fc7\u8be5\u9875\u9762\u65b0\u589e\u8986\u819c\u51fa\u6570\u89c4\u5219\u3002\r\n                    </p>\r\n                    <div class="form-horizontal form-label-left">\r\n                        <div class="form-group">\r\n                        <label class="control-label col-md-1 col-sm-1 col-xs-12">\u8303\u56f4</label>\r\n                        <div class="col-md-2 col-sm-2 col-xs-12">\r\n                            <input id="input_souce_begin" type="text" class="form-control" placeholder="">\r\n                        </div>\r\n                        <label class="control-label col-md-1 col-sm-1 col-xs-12">~</label>\r\n                        <div class="col-md-2 col-sm-2 col-xs-12">\r\n                            <input id="input_souce_end" type="text" class="form-control" placeholder="">\r\n                        </div>\r\n                        <label class="control-label col-md-1 col-sm-1 col-xs-12">\u6570\u91cf(\u4e2a)</label>\r\n                        <div class="col-md-2 col-sm-2 col-xs-12">\r\n                            <input id="input_souce_count" type="text" class="form-control" placeholder="">\r\n                        </div>\r\n                        </div>\r\n                        <div class="ln_solid">\r\n                        </div>\r\n                        <div class="form-group">\r\n                        <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\r\n                            <button id="submit_add_morerule" type="submit" class="btn btn-primary">\u65b0\u589e</button>\r\n                            </div>\r\n                        </div>\r\n                    </div>\r\n                </div>\r\n            </div>\r\n            </div>');
k=$("#submit_set_price");c=$("#input_impress_price");f=$("#table_filmmorerule");l=$("#submit_add_morerule");b=$("#input_souce_begin");d=$("#input_souce_end");e=$("#input_souce_count");n();h();r();t();g()};return m}},g_oFilmPriceConfig=UIFilmPriceConfig.createNew();