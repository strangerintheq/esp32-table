import * as React from 'preact';
import { useGeneratorStore } from '../../store/GeneratorStore/useGeneratorStore';
import './Generator.css';
import {PreviewCanvas} from "../../components/PreviewCanvas/PreviewCanvas";
import {useEffect} from "preact/compat";

export function Generator() {
    const {
        formula, turns, fixedStep, points, isLoading, status, isError, calculatedPointsCount,
        setField, setRandomFormula, generatePoints, uploadPoints
    } = useGeneratorStore();

    useEffect(() => {
        generatePoints();
    }, [generatePoints]);

    return (
        <div className="box max-wide compact-form">
            <h2>Pattern Generator</h2>

            {/* Поле формулы */}
            <div className="floating-group">
                <input
                    type="text"
                    value={formula}
                    placeholder=" " // Важно: пробел для работы CSS :placeholder-shown
                    onChange={(e: any) => setField('formula', e.target.value)}
                />
                <label>Radius formula r(a)</label>
            </div>
            <div className="hint">
                Angle: <b>a</b>. Negative processed as <code>Math.abs()</code>.
            </div>

            {/* Двухколоночный ряд для параметров */}
            <div className="input-row">
                <div className="floating-group">
                    <input
                        type="number"
                        value={turns}
                        min="0.1"
                        max="50"
                        step="0.1"
                        placeholder=" "
                        onChange={(e:any) => setField('turns', parseFloat(e.target.value) || 1)}
                    />
                    <label>Total Turns</label>
                </div>

                <div className="floating-group">
                    <input
                        type="number"
                        value={fixedStep}
                        min="0.005"
                        max="0.5"
                        step="0.005"
                        placeholder=" "
                        onChange={(e:any) => setField('fixedStep', parseFloat(e.target.value) || 0.05)}
                    />
                    <label>Fixed Step</label>
                </div>
            </div>

            <div className="compact-badge">
                Plotted points: <b>{calculatedPointsCount}</b>
            </div>

            {/* Компактный горизонтальный ряд кнопок */}
            <div className="btn-container-row">
                <button className="btn-rand" onClick={setRandomFormula} title="Random formula">🎲</button>
                <button className="btn-gen" onClick={generatePoints}>🔄 Calc</button>
                <button
                    className="btn-send"
                    onClick={uploadPoints}
                    disabled={points.length === 0 || isLoading || isError}
                >
                    🚀 Upload
                </button>
            </div>


            <PreviewCanvas points={points}/>

            {/* Статус ответа от ESP32 */}
            {status && (
                <div className={`status ${isError ? 'status-error' : 'status-success'}`}>
                    {status}
                </div>
            )}
        </div>
    );
}
