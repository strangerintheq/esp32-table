import * as React from 'preact';
import {PageWrapper} from "../../components/PageWrapper/PageWrapper";
import {useMemo} from "preact/compat";


const formulas = [
    ["(sin(i/180*PI)+0.5)*PI", `a/100+i*0.0003`],
    ["i/180*PI", "sin(a*2.5)*0.4+0.6"],
    ["i/180*PI", "sin(a*90)*0.49+0.51"],
    ["i/180*PI", "sin(a*2.1)*0.44+0.56"],
    ["i/180*PI", "sin(a*1.1)*0.44+0.56"],
    ["i/180*PI", `(0.8-a/100) / pow(pow(cos(a), 4) + pow(sin(a), 4), 0.25)`],
]

function FormulaDisplay(props: { fi: string, fa:string }) {

    const d = useMemo(() => {
        const fi = new Function("i", `{
            const {sqrt, sin, cos, tan, atan, atan2, log, pow, PI, round, ceil, floor} = Math;
            return ` + props.fi + ";\n}"
        );
        const fa = new Function("a", "i", `{
            const {sqrt, sin, cos, tan, atan, atan2, log, pow, PI, round, ceil, floor} = Math;
            return ` + props.fa + ";\n}"
        );
        const points = []
        for (let i = 0; i < 3600; i++) {
            const a = fi(i);
            const r = fa(a, i) * 45;
            const x = Math.cos(a) * r
            const y = Math.sin(a) * r
            points.push([x, y])
        }
        return "M" + points.join("L");
    }, [props.fi, props.fa]);

    return <svg viewBox={"-50 -50 100 100"} style={{pointerEvents:'none'}}>
        <path d={d} fill={'none'} stroke={"black"} stroke-width={0.5}/>
        <circle r={48} fill={'none'} stroke={"lightgray"}/>
    </svg>;
}

export function Gallery() {

    return (
        <PageWrapper title={"Gallery"}>
            <div style={{display: 'grid', gridTemplateColumns:'1fr 1fr 1fr', gap: 5}}>
                {formulas.map(x => {
                    return <FormulaDisplay fi={x[0]} fa={x[1]}/>
                })}
            </div>
        </PageWrapper>
    );
}
