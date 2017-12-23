var UIBoxCalculate={createNew:function(){function l(){a.html('<div class="col-md-12 col-sm-12 col-xs-12">                    <div class="x_panel">                        <div class="x_title">                            <h2>基础信息 <small>功能描述：通过此处输入基础参数信息。</small></h2>                            <div class="clearfix"></div>                        </div>                        <div class="x_content">                            <div class="form-horizontal form-label-left">                                <div class="form-group">                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">数量(个)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_count" type="text" class="form-control" placeholder="">                                    </div>                                </div>                                <div class="form-group">                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒盖长(mm)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_len" type="text" class="form-control" placeholder="">                                    </div>                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒盖宽(mm)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_width" type="text" class="form-control" placeholder="">                                    </div>                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒盖高(mm)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_height" type="text" class="form-control" placeholder="">                                    </div>                                </div>                                <div class="form-group">                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒底长(mm)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_len_x" type="text" class="form-control" placeholder="">                                    </div>                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒底宽(mm)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_width_x" type="text" class="form-control" placeholder="">                                    </div>                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒底高(mm)</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_height_x" type="text" class="form-control" placeholder="">                                    </div>                                </div>                            </div>                        </div>                    </div>                </div>                <div class="col-md-12 col-sm-12 col-xs-12">                    <div class="x_panel">                        <div class="x_title">                            <h2>材料选择 <small>功能描述：通过此处选择所需材料。</small></h2>                            <div class="clearfix"></div>                        </div>                        <div class="x_content">                            <div class="form-horizontal form-label-left">                                <div class = "form-group" >                                     <label class="control-label col-md-1 col-sm-1 col-xs-12">产品类型</label>                                    <div class="col-md-3 col-sm-3 col-xs-12">                                        <select id="select_product_type" class="form-control">                                            <option>礼品盒(盖)</option>                                            <option>礼品盒(底)</option>                                        </select>                                    </div>                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">制作材质</label>                                    <div class="col-md-3 col-sm-3 col-xs-12">                                        <select id="select_source_type" class="form-control">                                        </select>                                    </div>                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">材料用途</label>                                    <div class="col-md-3 col-sm-3 col-xs-12">                                        <select id="select_sourceuser_type" class="form-control">                                            <option>灰板</option>                                            <option>包纸</option>                                        </select>                                    </div>                                </div>                                <div class = "form-group" >                                     <label class="control-label col-md-1 col-sm-1 col-xs-12">是否烫金</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input class="check_green" type="checkbox" id="check_is_tangjin" checked="" required />                                    </div>                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">是否覆膜</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input class="check_green" type="checkbox" id="check_is_film" checked="" required />                                    </div>                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">是否印刷</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input class="check_green" type="checkbox" id="check_is_press" checked="" required />                                    </div>                                    <label id="label_press_type" class="control-label col-md-1 col-sm-1 col-xs-12">印刷方式</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <select id="select_press_type" class="form-control">                                        </select>                                    </div>                                </div>                                <div class = "form-group" >                                     <div class="col-md-1 col-sm-1 col-xs-12">                                        <button id = "submit_addsource" class = "btn btn-success" >增加材料</button>                                    </div>                                </div>                                <div class = "form-group" >                                     <table id="selectsource_table" class="table table-striped table-bordered">                                        <thead>                                            <tr>                                                <th>产品类型</th>                                                <th>材料种类</th>                                                <th>材料用途</th>                                                <th>是否烫金</th>                                                <th>是否覆膜</th>                                                <th>是否印刷</th>                                                <th>印刷方式</th>                                                <th>操作</th>                                            </tr>                                        </thead>                                        <tbody>                                        </tbody>                                    </table>                                 </div>                            </div>                        </div>                    </div>                </div>                <div class="col-md-12 col-sm-12 col-xs-12">                    <div class="x_panel">                        <div class="x_title">                            <h2>配件选择 <small>功能描述：通过此处选择所需配件。</small></h2>                            <div class="clearfix"></div>                        </div>                        <div class="x_content">                            <div class="form-horizontal form-label-left">                                <div class = "form-group" >                                         <div class="tabbable tabs-below">                                            <ul class="nav nav-tabs" id="tab_part_type">                                                <li class="active"><a href="#测试" data-toggle="tab">Section 1</a></li>                                                <li><a href="#btab2" data-toggle="tab">Section 2</a></li>                                                <li><a href="#btab3" data-toggle="tab">Section 3</a></li>                                                <li><a href="#btab4" data-toggle="tab">Section 4</a></li>                                                <li><a href="#btab5" data-toggle="tab">Section 5</a></li>                                                <li><a href="#btab6" data-toggle="tab">Section 6</a></li>                                                <li><a href="#btab7" data-toggle="tab">Section 7</a></li>                                                <li><a href="#btab8" data-toggle="tab">Section 8</a></li>                                                <li><a href="#btab9" data-toggle="tab">Section 9</a></li>                                            </ul>                                            <div class="tab-content form-group" id="tab_part_content">                                            </div>                                        </div>                                    <br>                                    <br>                                    <table id="selectpart_table" class="table table-striped table-bordered">                                        <thead>                                            <tr>                                                <th>配件名称</th>                                                <th>配件类别</th>                                            </tr>                                        </thead>                                        <tbody>                                        </tbody>                                    </table>                                </div>                            </div>                            </div>                        </div>                    </div>                </div>                <div class="col-md-12 col-sm-12 col-xs-12">                    <div class="x_panel">                        <div class="x_title">                            <h2>计算结果 <small>功能描述：计算包装盒的报价。</small></h2>                            <div class="clearfix"></div>                        </div>                        <div class="x_content">                            <div class="form-horizontal form-label-left">                                <div class = "form-group" >                                     <div class = "col-md-6 col-sm-6 col-xs-12" >                                        <button id = "submit_count" class = "btn btn-danger" >&nbsp;&nbsp;&nbsp;&nbsp;计&nbsp;&nbsp;算&nbsp;&nbsp;&nbsp;&nbsp;</button>                                    </div >                                     <label class="control-label col-md-1 col-sm-1 col-xs-12">总价</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_total_price" type="text" class="form-control" placeholder="" readonly>                                    </div>                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">单价</label>                                    <div class="col-md-2 col-sm-2 col-xs-12">                                        <input id="input_single_price" type="text" class="form-control" placeholder="" readonly>                                    </div>                                </div>                                <div class = "form-group" >                                     <div class = "col-md-12 col-sm-12 col-xs-12" >                                        <table id="log_table" class="table table-striped table-bordered">                                            <thead>                                                <tr>                                                    <th>类型</th>                                                    <th>日志</th>                                                </tr>                                            </thead>                                        </table>                                    </div >                                 </div>                            </div>                        </div>                    </div>                </div>            </div>            <div class="col-md-12 col-sm-12 col-xs-12">                <div class="x_panel">                    <div class="x_title">                        <h2>保存订单 <small>功能描述：保存订单客户信息，用于后续查询与下单。</small></h2>                        <div class="clearfix"></div>                    </div>                    <div class="x_content">                        <div class="form-horizontal form-label-left">                            <div class="form-group">                                <label class="control-label col-md-1 col-sm-1 col-xs-12">客户名称</label>                                <div class="col-md-8 col-sm-8 col-xs-12">                                    <input id="input_customer" type="text" class="form-control" placeholder="">                                </div>                                <label class="control-label col-md-1 col-sm-1 col-xs-12">最终总价</label>                                <div class="col-md-2 col-sm-2 col-xs-12">                                    <input id="input_finalprice" type="text" class="form-control" placeholder="">                                </div>                            </div>                        </div>                        <div class = "ln_solid" >                         </div>                        <div class="form-horizontal form-label-left">                            <div class = "form-group" >                                 <div class = "col-md-5 col-sm-5 col-xs-12 col-md-offset-1" >                                    <button id = "submit_save" class = "btn btn-danger" >&nbsp;&nbsp;&nbsp;&nbsp;保&nbsp;&nbsp;存&nbsp;&nbsp;&nbsp;&nbsp;</button>                                </div >                             </div>                        </div>                    </div >                 </div>            </div>        </div>'),o=$("#submit_count"),w=$("#submit_addsource"),s=$("#select_product_type"),e=$("#select_sourceuser_type"),c=$("#select_source_type"),r=$("#label_press_type"),i=$("#select_press_type"),n=$("#tab_part_type"),d=$("#tab_part_content"),p=$("#selectpart_table"),m=$("#selectsource_table"),v=$("#input_len"),u=$("#input_width"),b=$("#input_height"),x=$("#input_count"),g=$("#check_is_film"),y=$("#check_is_press"),h=$("#input_len_x"),f=$("#input_width_x"),_=$("#input_height_x"),k=$("#check_is_tangjin"),S=$("#input_customer"),T=$("#input_finalprice"),(N=$("#submit_save")).prop("disabled",!0),$("#check_is_press").on("ifChecked",function(l){r.show(),i.show()}).on("ifUnchecked",function(l){r.hide(),i.hide()}),$(".check_green").iCheck({checkboxClass:"icheckbox_flat-green"}),$(".icheckbox_flat-green").css("margin-top","8px"),I.length=0,j.length=0,$.ajax({type:"POST",url:"sourcetype-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(l){if(0==l.data.length)return c.html(""),void toastr.error("暂未配置制作材质信息，请前往配置！");for(var t="",a=0;a<l.data.length;a++){t+="<option>"+l.data[a].name+"</option>"}c.html(t)},error:function(){c.html(""),toastr.error("获取制作材质信息失败,请刷新重试！")}}),$.ajax({type:"POST",url:"presstype-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(l){if(0==l.data.length)return i.html(""),void toastr.error("暂未配置印刷方式信息，请前往配置！");for(var t="",a=0;a<l.data.length;a++){t+="<option>"+l.data[a].name+"</option>"}i.html(t)},error:function(){i.html(""),toastr.error("获取印刷方式信息失败,请刷新重试！")}}),$.ajax({type:"POST",url:"parttype-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(l){if(0==l.data.length)return n.html(""),d.html(""),void toastr.error("暂未配置配件种类信息，请前往配置！");for(var t="",a="",s=0;s<l.data.length;s++){if(0==s)var e="<li class='active'><a href='#"+l.data[s].name+"' data-toggle='tab'>"+l.data[s].name+"</a></li>",o="<div class='tab-pane active' id='"+l.data[s].name+"'></div>";else e="<li><a href='#"+l.data[s].name+"' data-toggle='tab'>"+l.data[s].name+"</a></li>",o="<div class='tab-pane' id='"+l.data[s].name+"'></div>";t+=e,a+=o}n.html(t),d.html(a),$(".nav-pills, .nav-tabs").tabdrop(),$.ajax({type:"POST",url:"part-all.zc",cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(l){if(0!=l.data.length){for(var t=0;t<l.data.length;t++)$("#"+l.data[t].type).append("<input class='check_aero' type='checkbox' name='"+l.data[t].name+"' alt='"+l.data[t].type+"' >&nbsp;"+l.data[t].name+"&nbsp;&nbsp;&nbsp;");!function(){if($(".check_aero").iCheck({checkboxClass:"icheckbox_flat-aero"}),$(".check_aero").on("ifChecked",function(l){var t=new Array;t.push(l.target.name),t.push(l.target.alt),I.push(t),p.DataTable({destroy:!0,data:I})}).on("ifUnchecked",function(l){for(var t=0;t<I.length;t++)if(I[t][0]==l.target.name&&I[t][1]==l.target.alt){I.splice(t,1);break}p.DataTable({destroy:!0,data:I})}),C.length>0){$(".check_aero").each(function(l,t){C.indexOf(t.name)>-1&&$(t).iCheck("check")})}}()}else toastr.error("暂未配置配件信息，可前往配置！")},error:function(){toastr.error("获取配件信息失败,请刷新重试！")}})},error:function(){n.html(""),d.html(""),toastr.error("获取配件种类信息失败,请刷新重试！")}}),w.on("click",function(){if(null!=c.val()){new Array;var l,t,a,o=null;if(l=k.is(":checked")?"烫金":"不烫金",t=g.is(":checked")?"覆膜":"不覆膜",y.is(":checked")?(a="印刷",o=i.val()):(a="不印刷",o="-"),null!=o){var r={pt:s.val().trim(),st:c.val().trim(),sut:e.val().trim(),btangjin:l,bfilm:t,bpress:a,presstype:o};j.push(r),m.DataTable({destroy:!0,data:j,columns:O}),m.on("click","a#delrow",function(l){l.preventDefault;var t=m.DataTable().row($(this).parents("tr")).data();swal({title:"提示",text:"确定要删除该条记录吗?",type:"warning",showCancelButton:!0,confirmButtonColor:"#DD6B55",confirmButtonText:"确定删除",cancelButtonText:"取消"},function(l){if(l){for(var a=0;a<j.length;a++)if(j[a].pt==t.pt&&j[a].st==t.st&&j[a].sut==t.sut&&j[a].btangjin==t.btangjin&&j[a].bfilm==t.bfilm&&j[a].bpress==t.bpress&&j[a].presstype==t.presstype){j.splice(a,1);break}m.DataTable({destroy:!0,data:j,columns:O})}})})}else toastr.error("暂未配置印刷方式信息，请前往配置！")}else toastr.error("暂未配置材料类型信息，请前往配置！")}),o.on("click",function(){if(isNull(v.val().trim()))toastr.error("盒盖长度不能为空！");else if(isInt(v.val().trim()))if(isNull(u.val().trim()))toastr.error("盒盖宽度不能为空！");else if(isInt(u.val().trim()))if(isNull(b.val().trim()))toastr.error("盒盖高度不能为空！");else if(isInt(b.val().trim()))if(isNull(h.val().trim()))toastr.error("盒底长度不能为空！");else if(isInt(h.val().trim()))if(isNull(f.val().trim()))toastr.error("盒底宽度不能为空！");else if(isInt(f.val().trim()))if(isNull(_.val().trim()))toastr.error("盒底高度不能为空！");else if(isInt(_.val().trim()))if(isNull(x.val().trim()))toastr.error("数量不能为空！");else if(isInt(x.val().trim())){for(var l=new Array,t=0;t<j.length;t++){var a=1;"礼品盒(盖)"==j[t].pt?a=1:"礼品盒(底)"==j[t].pt&&(a=2);var s=0;"灰板"==j[t].sut?s=0:"包纸"==j[t].sut&&(s=1);var e=!1;"烫金"==j[t].btangjin&&(e=!0);var o=!1;"覆膜"==j[t].bfilm&&(o=!0);var c=!1;"印刷"==j[t].bpress&&(c=!0);var r={l:1==a?parseInt(v.val().trim()):parseInt(h.val().trim()),w:1==a?parseInt(u.val().trim()):parseInt(f.val().trim()),h:1==a?parseInt(b.val().trim()):parseInt(_.val().trim()),producttype:parseInt(a),sourcetype:j[t].st.trim(),sourceusetype:parseInt(s),btangjin:e,bfilm:o,bpress:c,presstype:j[t].presstype.trim()};l.push(r)}var i=new Array;for(t=0;t<I.length;t++)i.push(I[t][0]);var n={count:parseInt(x.val().trim()),source:l,parts:i};$.ajax({type:"POST",url:"calculate-price.zc",cache:!1,data:JSON.stringify(n),dataType:"json",success:function(l){$("#input_total_price").val(l.totalprice),$("#input_single_price").val(l.price),$("#log_table").DataTable({ordering:!1,destroy:!0,data:l.log,createdRow:function(l,t,a){"错误"==t[0]?$("td",l).css("color","#f00"):"需求"==t[0]?$("td",l).css("color","#8080C0"):"材料"==t[0]?$("td",l).css("color","#00B271"):"印刷"==t[0]?$("td",l).css("color","#479AC7"):"覆膜"==t[0]?$("td",l).css("color","#B45B3E"):"压痕"==t[0]&&$("td",l).css("color","#6699CC")}}),N.removeAttr("disabled")},error:function(){toastr.error("向服务器请求失败,请稍后重试！")}})}else toastr.error("数量仅能输入数字！");else toastr.error("盒底高度仅能输入数字！");else toastr.error("盒底宽度仅能输入数字！");else toastr.error("盒底长度仅能输入数字！");else toastr.error("盒盖高度仅能输入数字！");else toastr.error("盒盖宽度仅能输入数字！");else toastr.error("盒盖长度仅能输入数字！")}),N.on("click",function(){if(isNull(S.val().trim()))toastr.error("客户名称不能为空！");else if(isNull(T.val().trim()))toastr.error("最终总价不能为空！");else if(isInt(T.val().trim())){for(var l=new Array,t=0;t<j.length;t++){var a=1;"礼品盒(盖)"==j[t].pt?a=1:"礼品盒(底)"==j[t].pt&&(a=2);var s=0;"灰板"==j[t].sut?s=0:"包纸"==j[t].sut&&(s=1);var e=!1;"烫金"==j[t].btangjin&&(e=!0);var o=!1;"覆膜"==j[t].bfilm&&(o=!0);var c=!1;"印刷"==j[t].bpress&&(c=!0);var r={l:1==a?parseInt(v.val().trim()):parseInt(h.val().trim()),w:1==a?parseInt(u.val().trim()):parseInt(f.val().trim()),h:1==a?parseInt(b.val().trim()):parseInt(_.val().trim()),producttype:parseInt(a),sourcetype:j[t].st.trim(),sourceusetype:parseInt(s),btangjin:e,bfilm:o,bpress:c,presstype:j[t].presstype.trim()};l.push(r)}var i=new Array;for(t=0;t<I.length;t++)i.push(I[t][0]);var n={count:parseInt(x.val().trim()),source:l,parts:i},d={customer:S.val().trim(),user:$("#welcome_tip").html().trim(),price:parseFloat($("#input_total_price").val().trim()),finalprice:parseFloat(T.val().trim()),condition:n};$.ajax({type:"POST",url:"order-add.zc",cache:!1,data:JSON.stringify(d),dataType:"json",success:function(l){toastr.success("订单数据保存成功"),N.prop("disabled",!0)},error:function(){toastr.error("向服务器请求失败,请稍后重试！")}})}else toastr.error("最终总价仅能输入数字！")})}var t={},a=$("#ui_body"),s=null,e=null,o=null,c=null,r=null,i=null,n=null,d=null,p=null,m=null,v=null,u=null,b=null,h=null,f=null,_=null,x=null,g=null,y=null,k=null,w=null,I=new Array,j=new Array,S=null,T=null,N=null,C=[],O=[{data:"pt"},{data:"st"},{data:"sut"},{data:"btangjin"},{data:"bfilm"},{data:"bpress"},{data:"presstype"},{data:null,className:"center",defaultContent:'<a id="delrow" href="#"><i class="fa fa-trash-o"></i>删除</a>'}];return t.load=function(){l()},t.loadByParam=function(t){l();var a=JSON.parse(t);x.val(a.count),C=a.parts;for(var s=0;s<a.source.length;++s){var e={pt:1==a.source[s].producttype?"礼品盒(盖)":"礼品盒(底)",st:a.source[s].sourcetype,sut:0==a.source[s].sourceusetype?"灰板":"包纸",btangjin:a.source[s].btangjin?"烫金":"不烫金",bfilm:a.source[s].bfilm?"覆膜":"不覆膜",bpress:a.source[s].bpress?"印刷":"不印刷",presstype:a.source[s].presstype};1==a.source[s].producttype?(v.val(a.source[s].l),u.val(a.source[s].w),b.val(a.source[s].h)):(h.val(a.source[s].l),f.val(a.source[s].w),_.val(a.source[s].h)),j.push(e)}m.DataTable({destroy:!0,data:j,columns:O})},t}},g_oBoxCalculate=UIBoxCalculate.createNew();