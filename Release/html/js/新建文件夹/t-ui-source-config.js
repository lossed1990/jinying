var UISourceConfig={createNew:function(){function q(){$.ajax({type:"POST",url:"sourcetype-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(a){if(0==a.data.length)e.html(""),toastr.error("\u6682\u672a\u914d\u7f6e\u6750\u6599\u7c7b\u522b\u4fe1\u606f\uff0c\u8bf7\u524d\u5f80\u914d\u7f6e\uff01");else{for(var h="",b=0;b<a.data.length;b++)h+="<option>"+a.data[b].name+"</option>";e.html(h)}},error:function(){e.html("");toastr.error("\u83b7\u53d6\u6750\u6599\u7c7b\u522b\u4fe1\u606f\u5931\u8d25,\u8bf7\u5237\u65b0\u91cd\u8bd5\uff01")}})}
function l(){k.dataTable({destroy:!0,ajax:function(a,h,b){$.ajax({type:"POST",url:"source-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(b){var c={};c.draw=a.draw;c.recordsTotal=b.recordsTotal;c.recordsFiltered=b.recordsFiltered;c.data=b.data;h(c)}})},columns:r})}function t(){m.on("click",function(){var a;if(isNull(b.val().trim()))toastr.error("\u65b0\u589e\u6750\u6599\u540d\u4e0d\u80fd\u4e3a\u7a7a\uff01");else if(isNull(c.val().trim()))toastr.error("\u65b0\u589e\u6750\u6599\u957f\u5ea6\u4e0d\u80fd\u4e3a\u7a7a\uff01");
else if(isInt(c.val().trim()))if(isNull(d.val().trim()))toastr.error("\u65b0\u589e\u6750\u6599\u5bbd\u5ea6\u4e0d\u80fd\u4e3a\u7a7a\uff01");else if(isInt(d.val().trim()))if(parseInt(c.val().trim())<parseInt(d.val().trim()))toastr.error("\u6750\u6599\u957f\u5ea6\u4e0d\u80fd\u5c0f\u4e8e\u5bbd\u5ea6\u503c\uff01");else if(isNull(f.val().trim()))toastr.error("\u65b0\u589e\u6750\u6599\u514b\u91cd\u4e0d\u80fd\u4e3a\u7a7a\uff01");else if(isInt(f.val().trim()))if(isNull(g.val().trim()))toastr.error("\u65b0\u589e\u6750\u6599\u5428\u4ef7\u4e0d\u80fd\u4e3a\u7a7a\uff01");
else if(isPrice(g.val().trim())){a=n.val().trim();if("\u624b\u63d0\u888b"==a)a={name:b.val().trim(),typename:e.val().trim(),l:parseInt(c.val().trim()),w:parseInt(d.val().trim()),kez:parseInt(f.val().trim()),dunj:parseFloat(g.val().trim()),usetype:0};else if("\u5305\u88c5\u76d2"==a)a={name:b.val().trim(),typename:e.val().trim(),l:parseInt(c.val().trim()),w:parseInt(d.val().trim()),kez:parseInt(f.val().trim()),dunj:parseFloat(g.val().trim()),usetype:1};else{toastr.error("\u8bf7\u9009\u62e9\u6750\u6599\u7528\u9014\uff01");
return}$.ajax({type:"POST",url:"source-add.zc",contentType:"application/json;charset=utf-8",data:JSON.stringify(a),dataType:"json",success:function(a){0==a.ok?(toastr.success("\u6750\u6599["+b.val().trim()+"]\u6dfb\u52a0\u6210\u529f"),l(),b.val(""),c.val(""),d.val(""),f.val(""),g.val("")):toastr.error("\u6750\u6599["+b.val().trim()+"]\u6dfb\u52a0\u5931\u8d25,"+a.errorinfo)},error:function(){toastr.error("\u6750\u6599\u6dfb\u52a0\u5931\u8d25,\u8bf7\u68c0\u67e5\u670d\u52a1\u5668\u53ca\u7f51\u7edc\u540e\u91cd\u8bd5\uff01")}})}else toastr.error("\u6750\u6599\u5428\u4ef7\u8f93\u5165\u4e0d\u5408\u6cd5\uff0c\u4ec5\u80fd\u7cbe\u786e\u5230\u5c0f\u6570\u70b9\u540e\u4e24\u4f4d\uff01");
else toastr.error("\u6750\u6599\u514b\u91cd\u4ec5\u80fd\u8f93\u5165\u6570\u5b57\uff01");else toastr.error("\u6750\u6599\u5bbd\u5ea6\u4ec5\u80fd\u8f93\u5165\u6570\u5b57\uff01");else toastr.error("\u6750\u6599\u957f\u5ea6\u4ec5\u80fd\u8f93\u5165\u6570\u5b57\uff01")})}function u(){k.on("click","a#delrow",function(a){a.preventDefault;var b=k.DataTable().row($(this).parents("tr")).data();swal({title:"\u63d0\u793a",text:"\u786e\u5b9a\u8981\u5220\u9664\u6750\u6599["+b.name+"]\u5417?",type:"warning",showCancelButton:!0,
confirmButtonColor:"#DD6B55",confirmButtonText:"\u786e\u5b9a\u5220\u9664",cancelButtonText:"\u53d6\u6d88"},function(a){a&&$.ajax({url:"source-del.zc",type:"POST",dataType:"json",data:JSON.stringify({name:b.name}),success:function(a){0==a.ok?(toastr.success("\u6750\u6599["+b.name+"]\u5220\u9664\u6210\u529f"),l()):toastr.error("\u6750\u6599["+b.name+"]\u5220\u9664\u5931\u8d25,"+a.errorinfo)},error:function(){toastr.error("\u6750\u6599\u5220\u9664\u5931\u8d25,\u8bf7\u68c0\u67e5\u670d\u52a1\u5668\u53ca\u7f51\u7edc\u540e\u91cd\u8bd5\uff01")}})})})}
var p={},v=$("#ui_body"),k=void 0,m=void 0,b=void 0,e=void 0,c=void 0,d=void 0,f=void 0,g=void 0,n=void 0,r=[{data:"name"},{data:"typename"},{data:"type"},{data:"kez"},{data:"dunj"},{data:"usetype"},{data:null,className:"center",defaultContent:'<a id="delrow" href="#"><i class="fa fa-trash-o"></i>\u5220\u9664</a>'}];p.load=function(){v.html('<div class="col-md-12 col-sm-12 col-xs-12">\r\n                <div class="x_panel">\r\n                <div class="x_title">\r\n                    <h2>\u603b\u89c8 <small>\u539f\u6750\u6599</small></h2>\r\n                    <div class="clearfix"></div>\r\n                </div>\r\n                <div class="x_content">\r\n                    <p class="text-muted font-13 m-b-30">\r\n                    \u529f\u80fd\u63cf\u8ff0\uff1a\u6b64\u5904\u5c55\u793a\u4e86\u76ee\u524d\u6240\u6709\u7684\u539f\u6750\u6599\u4fe1\u606f\uff0c\u60a8\u53ef\u901a\u8fc7\u8be5\u9875\u9762\u8fdb\u884c\u4fee\u6539\u4e0e\u5220\u9664\u3002\r\n                    </p>\r\n                    <table id="table_source" class="table table-striped table-bordered">\r\n                    <thead>\r\n                        <tr>\r\n                        <th>\u6750\u6599\u540d\u79f0</th>\r\n                        <th>\u6750\u6599\u7c7b\u522b</th>\r\n                        <th>\u89c4\u683c(mm*mm)</th>\r\n                        <th>\u514b\u91cd(g/m)</th>\r\n                        <th>\u5428\u4ef7(\u5143/\u5428)</th>\r\n                        <th>\u7528\u9014</th>\r\n                        <th>\u64cd\u4f5c</th>\r\n                        </tr>\r\n                    </thead>\r\n                    <tbody>\r\n                    </tbody>\r\n                    </table>\r\n                </div>\r\n                </div>\r\n            </div>\r\n            <div class="col-md-12 col-sm-12 col-xs-12">\r\n                <div class="x_panel">\r\n                    <div class="x_title">\r\n                        <h2>\u65b0\u589e <small>\u539f\u6750\u6599</small></h2>\r\n                        <div class="clearfix"></div>\r\n                    </div>\r\n                    <div class="x_content">\r\n                        <p class="text-muted font-13 m-b-30">\r\n                            \u529f\u80fd\u63cf\u8ff0\uff1a\u60a8\u53ef\u901a\u8fc7\u8be5\u9875\u9762\u65b0\u589e\u539f\u6750\u6599\u3002\r\n                        </p>\r\n                        <div class="form-horizontal form-label-left">\r\n                            <div class="form-group">\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u6750\u6599\u540d\u79f0</label>\r\n                            <div class="col-md-5 col-sm-5 col-xs-12">\r\n                                <input id="input_souce_name" type="text" class="form-control" placeholder="">\r\n                            </div>\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u6750\u6599\u7c7b\u522b</label>\r\n                            <div class="col-md-5 col-sm-5 col-xs-12">\r\n                                <select id="select_source_type" class="form-control">\r\n                                </select>\r\n                            </div>\r\n                            </div>\r\n                            <div class="form-group">\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u957f\u5ea6(mm)</label>\r\n                            <div class="col-md-2 col-sm-2 col-xs-12">\r\n                                <input id="input_souce_length" type="text" class="form-control" placeholder="">\r\n                            </div>\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u9ad8\u5ea6(mm)</label>\r\n                            <div class="col-md-2 col-sm-2 col-xs-12">\r\n                                <input id="input_souce_width" type="text" class="form-control" placeholder="">\r\n                            </div>\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u514b\u91cd(g/m)</label>\r\n                            <div class="col-md-2 col-sm-2 col-xs-12">\r\n                                <input id="input_souce_kezhong" type="text" class="form-control" placeholder="">\r\n                            </div>\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u5428\u4ef7(\u5143/\u5428)</label>\r\n                            <div class="col-md-2 col-sm-2 col-xs-12">\r\n                                <input id="input_souce_dunjia" type="text" class="form-control" placeholder="">\r\n                            </div>\r\n                            </div>\r\n                            <div class="form-group">\r\n                            <label class="control-label col-md-1 col-sm-1 col-xs-12">\u6750\u6599\u7528\u9014</label>\r\n                            <div class="col-md-11 col-sm-11 col-xs-12">\r\n                                <select id="select_use_type" class="form-control">\r\n                                <option>\u624b\u63d0\u888b</option>\r\n                                <option>\u5305\u88c5\u76d2</option>\r\n                                </select>\r\n                            </div>\r\n                            </div>\r\n                            <div class="ln_solid"></div>\r\n                            <div class="form-group">\r\n                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\r\n                                <button id="submit_add_source" class="btn btn-primary">\u65b0\u589e</button>\r\n                                </div>\r\n                            </div>\r\n                        </div>\r\n                    </div>\r\n                </div>\r\n            </div>');
k=$("#table_source");m=$("#submit_add_source");b=$("#input_souce_name");e=$("#select_source_type");c=$("#input_souce_length");d=$("#input_souce_width");f=$("#input_souce_kezhong");g=$("#input_souce_dunjia");n=$("#select_use_type");t();u();q();l()};return p}},g_oSourceConfig=UISourceConfig.createNew();