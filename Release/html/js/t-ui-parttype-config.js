var UIParttypeConfig={createNew:function(){function t(){l.dataTable({destroy:!0,ajax:function(t,a,e){$.ajax({type:"POST",url:s,cache:!1,data:JSON.stringify("{'get':'all'}"),dataType:"json",success:function(e){var l={};l.draw=t.draw,l.recordsTotal=e.recordsTotal,l.recordsFiltered=e.recordsFiltered,l.data=e.data,a(l)}})},columns:d})}var a={},e=$("#ui_body"),l=void 0,r=void 0,o=void 0,s="parttype-all.zc",i="parttype-add.zc",c="parttype-del.zc",d=[{data:"name"},{data:null,className:"center",defaultContent:'<a id="delrow" href="#"><i class="fa fa-trash-o"></i>删除</a>'}];return a.load=function(){e.html('<div class="col-md-12 col-sm-12 col-xs-12">                <div class="x_panel">                    <div class="x_title">                    <h2>总览 <small>配件种类</small></h2>                    <div class="clearfix"></div>                    </div>                    <div class="x_content">                    <p class="text-muted font-13 m-b-30">                        功能描述：此处展示了目前所有的配件种类信息，您可通过该页面进行修改与删除。                    </p>                    <table id="table_parttype" class="table table-striped table-bordered">                        <thead>                        <tr>                            <th>配件种类名称</th>                            <th>操作</th>                        </tr>                        </thead>                        <tbody>                        </tbody>                    </table>                    </div>                </div>            </div>            <div class="col-md-12 col-sm-12 col-xs-12">                <div class="x_panel">                    <div class="x_title">                        <h2>新增 <small>配件种类</small></h2>                        <div class="clearfix"></div>                    </div>                    <div class="x_content">                        <p class="text-muted font-13 m-b-30">                            功能描述：您可通过该页面新增配件种类。                        </p>                        <div class="form-horizontal form-label-left">                            <div class="form-group">                            <label class="control-label col-md-1 col-sm-1 col-xs-12">种类名称</label>                            <div class="col-md-11 col-sm-11 col-xs-12">                                <input id="input_parttype_name" type="text" class="form-control" placeholder="">                            </div>                            </div>                            <div class="ln_solid">                            </div>                            <div class="form-group">                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">                                <button id="submit_add_parttype" type="submit" class="btn btn-primary">新增</button>                                </div>                            </div>                        </div>                    </div>                </div>            </div>'),l=$("#table_parttype"),r=$("#submit_add_parttype"),o=$("#input_parttype_name"),r.on("click",function(){var a;isNull(o.val().trim())?toastr.error("配件种类名称不能为空！"):(a={name:o.val().trim()},$.ajax({type:"POST",url:i,contentType:"application/json;charset=utf-8",data:JSON.stringify(a),dataType:"json",success:function(a){0==a.ok?(toastr.success("配件种类["+o.val().trim()+"]添加成功"),t(),o.val("")):toastr.error("配件种类["+o.val().trim()+"]添加失败,"+a.errorinfo)},error:function(){toastr.error("配件种类添加失败,请检查服务器及网络后重试！")}}))}),l.on("click","a#delrow",function(a){a.preventDefault;var e=l.DataTable().row($(this).parents("tr")).data(),r="确定要删除配件种类["+e.name+"]吗?";swal({title:"提示",text:r,type:"warning",showCancelButton:!0,confirmButtonColor:"#DD6B55",confirmButtonText:"确定删除",cancelButtonText:"取消"},function(a){if(a){var l={name:e.name};$.ajax({url:c,type:"POST",dataType:"json",data:JSON.stringify(l),success:function(a){0==a.ok?(toastr.success("配件种类["+e.name+"]删除成功"),t()):toastr.error("配件种类["+e.name+"]删除失败,"+a.errorinfo)},error:function(){toastr.error("配件种类删除失败,请检查服务器及网络后重试！")}})}})}),t()},a}},g_oParttypeConfig=UIParttypeConfig.createNew();