within SorpPropLib;
package DirectFunctionCalls "Package defining functions based on direct function calls (i.e., slow approach used for parameter calculation)"
  extends Modelica.Icons.Package;

annotation (Documentation(revisions="<html>
<ul>
  <li>
  July 13, 2021, by Mirko Engelpracht:<br/>
  First implementation.
  </li>
</ul>
</html>", info="<html>
</html>"), Icon(graphics={
        Ellipse(
          lineColor = {108,88,49},
          fillColor = {255,215,136},
          fillPattern = FillPattern.Solid,
          extent={{-80,-80},{80,80}}),
        Text(
          lineColor={108,88,49},
          extent={{-90,-90},{90,90}},
          textString="f")}));
end DirectFunctionCalls;
