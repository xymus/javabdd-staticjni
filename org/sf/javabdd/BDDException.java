package org.sf.javabdd;

/**
 * An exception caused by an invalid BDD operation.
 * 
 * @author John Whaley
 * @version $Id: BDDException.java,v 1.2 2003/01/30 06:22:18 joewhaley Exp $
 */
public class BDDException extends Exception {
    public BDDException() {
        super();
    }
    public BDDException(String s) {
        super(s);
    }
}
