package org.sf.javabdd;

import java.util.Collection;
import java.util.Iterator;

/**
 * @author John Whaley
 */
public class BuDDyFactory extends BDDFactory {

    public static BDDFactory init(int nodenum, int cachesize) {
        if (INSTANCE != null) {
            throw new InternalError("Error: BDDFactory already initialized.");
        }
        INSTANCE = new BuDDyFactory();
        INSTANCE.initialize(nodenum, cachesize);
        return INSTANCE;
    }
    
    private static BuDDyFactory INSTANCE;

    static {
        System.loadLibrary("buddy");
        registerNatives();
    }
    
    private static native void registerNatives();
    
    private BuDDyFactory() {}

    /**
     * @see org.sf.javabdd.BDDFactory#zero()
     */
    public BDD zero() { return _getZero(); }

    protected native BuDDyBDD _getZero();

    /**
     * @see org.sf.javabdd.BDDFactory#one()
     */
    public BDD one() { return _getOne(); }

    protected native BuDDyBDD _getOne();
    
    /**
     * @see org.sf.javabdd.BDDFactory#buildCube(int, int, java.util.Collection)
     */
    public native BDD buildCube(int value, int width, Collection var);

    /**
     * @see org.sf.javabdd.BDDFactory#buildCube(int, int, int)
     */
    public native BDD buildCube(int value, int width, int[] var);

    /**
     * @see org.sf.javabdd.BDDFactory#makeSet(int)
     */
    public native BDD makeSet(int[] v);
    
    /**
     * @see org.sf.javabdd.BDDFactory#initialize(int, int)
     */
    protected native void initialize(int nodenum, int cachesize);

    /**
     * @see org.sf.javabdd.BDDFactory#isInitialized()
     */
    public native boolean isInitialized();

    /**
     * @see org.sf.javabdd.BDDFactory#done()
     */
    public void done() {
        INSTANCE = null;
        _done();
    }

    protected native void _done();

    /**
     * @see org.sf.javabdd.BDDFactory#setMaxNodeNum(int)
     */
    public native int setMaxNodeNum(int size);

    /**
     * @see org.sf.javabdd.BDDFactory#setMinFreeNodes(int)
     */
    public native void setMinFreeNodes(int x);

    /**
     * @see org.sf.javabdd.BDDFactory#setMaxIncrease(int)
     */
    public native int setMaxIncrease(int x);

    /**
     * @see org.sf.javabdd.BDDFactory#setCacheRatio(int)
     */
    public native int setCacheRatio(int x);

    /**
     * @see org.sf.javabdd.BDDFactory#varNum()
     */
    public native int varNum();

    /**
     * @see org.sf.javabdd.BDDFactory#setVarNum(int)
     */
    public native int setVarNum(int num);

    /**
     * @see org.sf.javabdd.BDDFactory#extVarNum(int)
     */
    public native int extVarNum(int num);

    /**
     * @see org.sf.javabdd.BDDFactory#ithVar(int)
     */
    public native BDD ithVar(int var);

    /**
     * @see org.sf.javabdd.BDDFactory#nithVar(int)
     */
    public native BDD nithVar(int var);

    /**
     * @see org.sf.javabdd.BDDFactory#swapVar(int, int)
     */
    public native void swapVar(int v1, int v2);

    /**
     * @see org.sf.javabdd.BDDFactory#printAll()
     */
    public native void printAll();

    /**
     * @see org.sf.javabdd.BDDFactory#printTable(org.sf.javabdd.BDD)
     */
    public native void printTable(BDD b);

    /**
     * @see org.sf.javabdd.BDDFactory#load(java.lang.String)
     */
    public native BDD load(String filename);

    /**
     * @see org.sf.javabdd.BDDFactory#save(java.lang.String)
     */
    public native void save(String filename, BDD v);

    /**
     * @see org.sf.javabdd.BDDFactory#level2Var(int)
     */
    public native int level2Var(int level);

    /**
     * @see org.sf.javabdd.BDDFactory#var2level(int)
     */
    public native int var2level(int var);

    /**
     * @see org.sf.javabdd.BDDFactory#reorder(org.sf.javabdd.BDDFactory.ReorderMethod)
     */
    public native void reorder(BDDFactory.ReorderMethod m);

    /**
     * @see org.sf.javabdd.BDDFactory#autoReorder(org.sf.javabdd.BDDFactory.ReorderMethod)
     */
    public native void autoReorder(BDDFactory.ReorderMethod method);

    /**
     * @see org.sf.javabdd.BDDFactory#autoReorder(org.sf.javabdd.BDDFactory.ReorderMethod, int)
     */
    public native void autoReorder(BDDFactory.ReorderMethod method, int max);

    /**
     * @see org.sf.javabdd.BDDFactory#getReorderMethod()
     */
    public native BDDFactory.ReorderMethod getReorderMethod();

    /**
     * @see org.sf.javabdd.BDDFactory#getReorderTimes()
     */
    public native int getReorderTimes();

    /**
     * @see org.sf.javabdd.BDDFactory#disableReorder()
     */
    public native void disableReorder();

    /**
     * @see org.sf.javabdd.BDDFactory#enableReorder()
     */
    public native void enableReorder();

    /**
     * @see org.sf.javabdd.BDDFactory#reorderVerbose(int)
     */
    public native int reorderVerbose(int v);

    /**
     * @see org.sf.javabdd.BDDFactory#addVarBlock(org.sf.javabdd.BDD, boolean)
     */
    public native void addVarBlock(BDD var, boolean fixed);

    /**
     * @see org.sf.javabdd.BDDFactory#addVarBlock(int, int, boolean)
     */
    public native void addVarBlock(int first, int last, boolean fixed);

    /**
     * @see org.sf.javabdd.BDDFactory#varBlockAll()
     */
    public native void varBlockAll();

    /**
     * @see org.sf.javabdd.BDDFactory#clearVarBlocks()
     */
    public native void clearVarBlocks();

    /**
     * @see org.sf.javabdd.BDDFactory#printOrder()
     */
    public native void printOrder();

    /**
     * @see org.sf.javabdd.BDDFactory#nodeCount(java.util.Collection)
     */
    public native int nodeCount(Collection r);

    /**
     * @see org.sf.javabdd.BDDFactory#getAllocNum()
     */
    public native int getAllocNum();

    /**
     * @see org.sf.javabdd.BDDFactory#getNodeNum()
     */
    public native int getNodeNum();

    /**
     * @see org.sf.javabdd.BDDFactory#reorderGain()
     */
    public native int reorderGain();

    /**
     * @see org.sf.javabdd.BDDFactory#printStat()
     */
    public native void printStat();

    /**
     * @see org.sf.javabdd.BDDFactory#extDomain()
     */
    public native void extDomain();

    public static class BuDDyBDD extends BDD {
    
        private int _id;
        
        private BuDDyBDD(int id) {
            this._id = id;
            this.addRef();
        }
        
        /**
         * @see org.sf.javabdd.BDD#var()
         */
        public native int var();
        
        /**
         * @see org.sf.javabdd.BDD#high()
         */
        public native BDD high();
        
        /**
         * @see org.sf.javabdd.BDD#low()
         */
        public native BDD low();
        
        /**
         * @see org.sf.javabdd.BDD#id()
         */
        public native BDD id();
        
        /**
         * @see org.sf.javabdd.BDD#not()
         */
        public native BDD not();
        
        /**
         * @see org.sf.javabdd.BDD#ite(org.sf.javabdd.BDD, org.sf.javabdd.BDD)
         */
        public native BDD ite(BDD thenBDD, BDD elseBDD);
        
        /**
         * @see org.sf.javabdd.BDD#relprod(org.sf.javabdd.BDD, org.sf.javabdd.BDD)
         */
        public native BDD relprod(BDD that, BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#compose(org.sf.javabdd.BDD, int)
         */
        public native BDD compose(BDD that, int var);
        
        /**
         * @see org.sf.javabdd.BDD#constrain(org.sf.javabdd.BDD)
         */
        public native BDD constrain(BDD that);
        
        /**
         * @see org.sf.javabdd.BDD#exist(org.sf.javabdd.BDD)
         */
        public native BDD exist(BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#forAll(org.sf.javabdd.BDD)
         */
        public native BDD forAll(BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#unique(org.sf.javabdd.BDD)
         */
        public native BDD unique(BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#restrict(org.sf.javabdd.BDD)
         */
        public native BDD restrict(BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#simplify(org.sf.javabdd.BDD)
         */
        public native BDD simplify(BDD d);
        
        /**
         * @see org.sf.javabdd.BDD#support()
         */
        public native BDD support();
        
        /**
         * @see org.sf.javabdd.BDD#apply(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp)
         */
        public native BDD apply(BDD that, BDDFactory.BDDOp opr);
        
        /**
         * @see org.sf.javabdd.BDD#applyWith(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp)
         */
        public native void applyWith(BDD that, BDDFactory.BDDOp opr);
        
        /**
         * @see org.sf.javabdd.BDD#applyAll(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp, org.sf.javabdd.BDD)
         */
        public native BDD applyAll(BDD that, BDDFactory.BDDOp opr, BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#applyEx(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp, org.sf.javabdd.BDD)
         */
        public native BDD applyEx(BDD that, BDDFactory.BDDOp opr, BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#applyUni(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp, org.sf.javabdd.BDD)
         */
        public native BDD applyUni(BDD that, BDDFactory.BDDOp opr, BDD var);
        
        /**
         * @see org.sf.javabdd.BDD#satOne()
         */
        public native BDD satOne();
        
        /**
         * @see org.sf.javabdd.BDD#fullSatOne()
         */
        public native BDD fullSatOne();
        
        /**
         * @see org.sf.javabdd.BDD#satOneSet(org.sf.javabdd.BDD, org.sf.javabdd.BDD)
         */
        public native BDD satOneSet(BDD var, BDD pol);
        
        /**
         * @see org.sf.javabdd.BDD#allsat()
         */
        public Iterator allsat() {
            return null;
        }
        
        /**
         * @see org.sf.javabdd.BDD#printSet()
         */
        public native void printSet();
        
        /**
         * @see org.sf.javabdd.BDDFactory#printDot()
         */
        public native void printDot();

        /**
         * @see org.sf.javabdd.BDD#nodeCount()
         */
        public native int nodeCount();
        
        /**
         * @see org.sf.javabdd.BDD#pathCount()
         */
        public native double pathCount();
        
        /**
         * @see org.sf.javabdd.BDD#satCount()
         */
        public native double satCount();
        
        /**
         * @see org.sf.javabdd.BDD#satCount(org.sf.javabdd.BDD)
         */
        public native double satCount(BDD varset);
        
        /**
         * @see org.sf.javabdd.BDD#logSatCount()
         */
        public native double logSatCount();
        
        /**
         * @see org.sf.javabdd.BDD#logSatCount(org.sf.javabdd.BDD)
         */
        public native double logSatCount(BDD varset);
        
        /**
         * @see org.sf.javabdd.BDD#addRef()
         */
        protected native void addRef();
        
        /**
         * @see org.sf.javabdd.BDD#delRef()
         */
        protected native void delRef();
        
    }

}
