export interface GeneratorStore {
    formula: string;
    turns: number;
    fixedStep: number;        // Фиксированное расстояние между точками (например, 0.05)
    points: [number, number][];
    isLoading: boolean;
    status: string | null;
    isError: boolean;
    calculatedPointsCount: number;

    setField: (field: 'formula' | 'turns' | 'fixedStep', value: string | number) => void;
    setRandomFormula: () => void;
    generatePoints: () => void;
    uploadPoints: () => Promise<void>;
}