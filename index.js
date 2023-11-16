import * as stockchart from "./build/stockchart.js"

stockchart._init(stringToNewUTF8("myCanvas"))
stockchart._draw(ctx)
stockchart._cleanup(ctx)
