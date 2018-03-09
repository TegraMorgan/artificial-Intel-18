package lab1;

public enum CAR_TYPE {
    A('A'), B('B'),C('C'),X('X'),O('O'),P('P'),Q('Q'),R('R');

    public char asChar() {
        return asChar;
    }

    private final char asChar;

    private CAR_TYPE(char asChar) {
        this.asChar = asChar;
    }
}