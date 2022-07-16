public enum days {
    // Defined with properties - which day of the week and whether a weekday or not
    MONDAY (2, true),
    TUESDAY (3, true),
    WEDNESDAY (4, true),
    THURSDAY (5, true),
    FRIDAY (6, true),
    SATURDAY (7, false),
    SUNDAY (1, false);
    
    final int birthday;
    final boolean weekday;
    
    days(int bday, boolean wkday) {
        birthday = bday;
        weekday = wkday;
    }
    
    public int getBirthday() {
        return birthday;
    }
    
    public boolean getWeekday() {
        return weekday;
    }
}
