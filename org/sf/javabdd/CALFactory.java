/*
 * CALFactory.java
 * 
 * Created on Mar 24, 2003
 *
 */
package org.sf.javabdd;

import java.util.Collection;
import java.util.List;

/**
 * <p>An implementation of BDDFactory that relies on the CAL library through a
 * native interface.  You can use this by calling the "CALFactory.init()"
 * method with the desired arguments.  This will return you an instance of the
 * BDDFactory class that you can use.  Call "done()" on that instance when you
 * are finished.</p>
 * 
 * <p>CAL does not have much of the functionality that BuDDy has, and it has
 * not been well-tested.  Furthermore, it is slower than BuDDy.  Therefore, it
 * is recommended that you use the BuDDy library instead.</p>
 * 
 * <p>This class (and the CAL library) do NOT support multithreading.
 * Furthermore, there can be only one instance active at a time.  You can only
 * call "init()" again after you have called "done()" on the original instance.
 * It is not recommended to call "init()" again after calling "done()" unless
 * you are _completely_ sure that all BDD objects that reference the old
 * factory have been freed.</p>
 * 
 * <p>If you really need multiple BDD factories, consider using the JavaFactory
 * class for the additional BDD factories --- JavaFactory can have multiple
 * factory instances active at a time.</p>
 * 
 * @see org.sf.javabdd.BDDFactory
 * @see org.sf.javabdd.BuDDyFactory
 * 
 * @author John Whaley
 * @version $Id: CALFactory.java,v 1.5 2004/08/02 20:20:53 joewhaley Exp $
 */
public class CALFactory extends BDDFactory {

    public static BDDFactory init(int nodenum, int cachesize) {
        if (INSTANCE != null) {
            throw new InternalError("Error: CALFactory already initialized.");
        }
        INSTANCE = new CALFactory();
        INSTANCE.initialize(nodenum/256, cachesize);
        return INSTANCE;
    }
    
    private static CALFactory INSTANCE;
    
    static {
        String libname = "cal";
        try {
            System.loadLibrary(libname);
        } catch (java.lang.UnsatisfiedLinkError x) {
            // Cannot find library, try loading it from the current directory...
            libname = System.mapLibraryName(libname);
            String currentdir = System.getProperty("user.dir");
            String sep = System.getProperty("file.separator");
            System.load(currentdir+sep+libname);
        }
        registerNatives();
    }
    
    private static native void registerNatives();
    
    private CALFactory() {}
    
    private static long zero;
    private static long one;
    
    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#zero()
     */
    public BDD zero() {
        return new CALBDD(zero);
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#one()
     */
    public BDD one() {
        return new CALBDD(one);
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#initialize(int, int)
     */
    protected void initialize(int nodenum, int cachesize) {
        initialize0(nodenum, cachesize);
    }
    private static native void initialize0(int nodenum, int cachesize);

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#isInitialized()
     */
    public boolean isInitialized() {
        return isInitialized0();
    }
    private static native boolean isInitialized0();

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#done()
     */
    public void done() {
        INSTANCE = null;
        done0();
    }
    private static native void done0();

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#setMaxNodeNum(int)
     */
    public int setMaxNodeNum(int size) {
        // TODO Implement this.
        System.err.println("Warning: setMaxNodeNum() not yet implemented");
        return 1000000;
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#setMinFreeNodes(int)
     */
    public void setMinFreeNodes(int x) {
        // TODO Implement this.
        System.err.println("Warning: setMinFreeNodes() not yet implemented");
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#setMaxIncrease(int)
     */
    public int setMaxIncrease(int x) {
        // TODO Implement this.
        System.err.println("Warning: setMaxIncrease() not yet implemented");
        return 50000;
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#setCacheRatio(int)
     */
    public int setCacheRatio(int x) {
        // TODO Implement this.
        System.err.println("Warning: setCacheRatio() not yet implemented");
        return 0;
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#varNum()
     */
    public int varNum() {
        return varNum0();
    }
    private static native int varNum0();

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#setVarNum(int)
     */
    public int setVarNum(int num) {
        return setVarNum0(num);
    }
    private static native int setVarNum0(int num);

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#ithVar(int)
     */
    public BDD ithVar(int var) {
        long id = ithVar0(var);
        return new CALBDD(id);
    }
    private static native long ithVar0(int var);

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#nithVar(int)
     */
    public BDD nithVar(int var) {
        BDD b = ithVar(var);
        BDD c = b.not(); b.free();
        return c;
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#swapVar(int, int)
     */
    public void swapVar(int v1, int v2) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#makePair()
     */
    public BDDPairing makePair() {
        return new CALBDDPairing();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#printAll()
     */
    public void printAll() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#printTable(org.sf.javabdd.BDD)
     */
    public void printTable(BDD b) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#level2Var(int)
     */
    public int level2Var(int level) {
        return level2Var0(level);
    }
    private static native int level2Var0(int level);

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#var2Level(int)
     */
    public int var2Level(int var) {
        return var2Level0(var);
    }
    private static native int var2Level0(int var);

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#reorder(org.sf.javabdd.BDDFactory.ReorderMethod)
     */
    public void reorder(ReorderMethod m) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#autoReorder(org.sf.javabdd.BDDFactory.ReorderMethod)
     */
    public void autoReorder(ReorderMethod method) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#autoReorder(org.sf.javabdd.BDDFactory.ReorderMethod, int)
     */
    public void autoReorder(ReorderMethod method, int max) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#getReorderMethod()
     */
    public ReorderMethod getReorderMethod() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#getReorderTimes()
     */
    public int getReorderTimes() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#disableReorder()
     */
    public void disableReorder() {
        // TODO Implement this.
        System.err.println("Warning: disableReorder() not yet implemented");
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#enableReorder()
     */
    public void enableReorder() {
        // TODO Implement this.
        System.err.println("Warning: enableReorder() not yet implemented");
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#reorderVerbose(int)
     */
    public int reorderVerbose(int v) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#setVarOrder(int[])
     */
    public void setVarOrder(int[] neworder) {
        setVarOrder0(neworder);
    }
    private static native void setVarOrder0(int[] neworder);

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#addVarBlock(org.sf.javabdd.BDD, boolean)
     */
    public void addVarBlock(BDD var, boolean fixed) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#addVarBlock(int, int, boolean)
     */
    public void addVarBlock(int first, int last, boolean fixed) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#varBlockAll()
     */
    public void varBlockAll() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#clearVarBlocks()
     */
    public void clearVarBlocks() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#printOrder()
     */
    public void printOrder() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#nodeCount(java.util.Collection)
     */
    public int nodeCount(Collection r) {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#getAllocNum()
     */
    public int getAllocNum() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#getNodeNum()
     */
    public int getNodeNum() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#reorderGain()
     */
    public int reorderGain() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#printStat()
     */
    public void printStat() {
        // TODO Implement this.
        throw new UnsupportedOperationException();
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#createDomain(int, long)
     */
    protected BDDDomain createDomain(int a, long b) {
        return new CALBDDDomain(a, b);
    }

    /* (non-Javadoc)
     * An implementation of a BDD class, used by the CAL interface.
     */
    private static class CALBDD extends BDD {

        /** The pointer used by the BDD library. */
        private long _ddnode_ptr;
        
        /** An invalid id, for use in invalidating BDDs. */
        static final long INVALID_BDD = -1;
        
        private CALBDD(long ddnode) {
            this._ddnode_ptr = ddnode;
            addRef(ddnode);
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#getFactory()
         */
        public BDDFactory getFactory() {
            return INSTANCE;
        }
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#isZero()
         */
        public boolean isZero() {
            return this._ddnode_ptr == zero;
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#isOne()
         */
        public boolean isOne() {
            return this._ddnode_ptr == one;
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#var()
         */
        public int var() {
            return var0(_ddnode_ptr);
        }
        private static native int var0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#high()
         */
        public BDD high() {
            long b = high0(_ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long high0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#low()
         */
        public BDD low() {
            long b = low0(_ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long low0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#id()
         */
        public BDD id() {
            return new CALBDD(_ddnode_ptr);
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#not()
         */
        public BDD not() {
            long b = not0(_ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long not0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#ite(org.sf.javabdd.BDD, org.sf.javabdd.BDD)
         */
        public BDD ite(BDD thenBDD, BDD elseBDD) {
            CALBDD c = (CALBDD) thenBDD;
            CALBDD d = (CALBDD) elseBDD;
            long b = ite0(_ddnode_ptr, c._ddnode_ptr, d._ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long ite0(long b, long c, long d);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#relprod(org.sf.javabdd.BDD, org.sf.javabdd.BDD)
         */
        public BDD relprod(BDD that, BDD var) {
            CALBDD c = (CALBDD) that;
            CALBDD d = (CALBDD) var;
            long b = relprod0(_ddnode_ptr, c._ddnode_ptr, d._ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long relprod0(long b, long c, long d);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#compose(org.sf.javabdd.BDD, int)
         */
        public BDD compose(BDD that, int var) {
            CALBDD c = (CALBDD) that;
            long b = compose0(_ddnode_ptr, c._ddnode_ptr, var);
            return new CALBDD(b);
        }
        private static native long compose0(long b, long c, int var);

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#constrain(org.sf.javabdd.BDD)
         */
        public BDD constrain(BDD that) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#exist(org.sf.javabdd.BDD)
         */
        public BDD exist(BDD var) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#forAll(org.sf.javabdd.BDD)
         */
        public BDD forAll(BDD var) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#unique(org.sf.javabdd.BDD)
         */
        public BDD unique(BDD var) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#restrict(org.sf.javabdd.BDD)
         */
        public BDD restrict(BDD var) {
            CALBDD c = (CALBDD) var;
            long b = restrict0(_ddnode_ptr, c._ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long restrict0(long b, long var);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#restrictWith(org.sf.javabdd.BDD)
         */
        public BDD restrictWith(BDD var) {
            CALBDD c = (CALBDD) var;
            long b = restrict0(_ddnode_ptr, c._ddnode_ptr);
            addRef(b);
            delRef(_ddnode_ptr);
            if (this != c) {
                delRef(c._ddnode_ptr);
                c._ddnode_ptr = INVALID_BDD;
            }
            _ddnode_ptr = b;
            return this;
        }
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#simplify(org.sf.javabdd.BDD)
         */
        public BDD simplify(BDD d) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#support()
         */
        public BDD support() {
            long b = support0(_ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long support0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#apply(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp)
         */
        public BDD apply(BDD that, BDDFactory.BDDOp opr) {
            CALBDD c = (CALBDD) that;
            long b = apply0(_ddnode_ptr, c._ddnode_ptr, opr.id);
            return new CALBDD(b);
        }
        private static native long apply0(long b, long c, int opr);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#applyWith(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp)
         */
        public BDD applyWith(BDD that, BDDFactory.BDDOp opr) {
            CALBDD c = (CALBDD) that;
            long b = apply0(_ddnode_ptr, c._ddnode_ptr, opr.id);
            addRef(b);
            delRef(_ddnode_ptr);
            if (this != c) {
                delRef(c._ddnode_ptr);
                c._ddnode_ptr = INVALID_BDD;
            }
            _ddnode_ptr = b;
            return this;
        }
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#applyAll(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp, org.sf.javabdd.BDD)
         */
        public BDD applyAll(BDD that, BDDOp opr, BDD var) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#applyEx(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp, org.sf.javabdd.BDD)
         */
        public BDD applyEx(BDD that, BDDOp opr, BDD var) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#applyUni(org.sf.javabdd.BDD, org.sf.javabdd.BDDFactory.BDDOp, org.sf.javabdd.BDD)
         */
        public BDD applyUni(BDD that, BDDOp opr, BDD var) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#satOne()
         */
        public BDD satOne() {
            long b = satOne0(_ddnode_ptr);
            return new CALBDD(b);
        }
        private static native long satOne0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#fullSatOne()
         */
        public BDD fullSatOne() {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#satOne(org.sf.javabdd.BDD, org.sf.javabdd.BDD)
         */
        public BDD satOne(BDD var, BDD pol) {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#allsat()
         */
        public List allsat() {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#nodeCount()
         */
        public int nodeCount() {
            return nodeCount0(_ddnode_ptr);
        }
        private static native int nodeCount0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#pathCount()
         */
        public double pathCount() {
            return pathCount0(_ddnode_ptr);
        }
        private static native double pathCount0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#satCount()
         */
        public double satCount() {
            return satCount0(_ddnode_ptr);
        }
        // TODO: debug CAL satCount.
        private static native double satCount0(long b);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#varProfile()
         */
        public int[] varProfile() {
            // TODO Implement this.
            throw new UnsupportedOperationException();
        }

        private static native void addRef(long p);

        private static native void delRef(long p);
        
        static final boolean USE_FINALIZER = false;
        
        /* Finalizer runs in different thread, and CAL is not thread-safe.
         * Also, the existence of any finalize() method hurts performance
         * considerably.
         */
        /* (non-Javadoc)
         * @see java.lang.Object#finalize()
         */
        /*
        protected void finalize() throws Throwable {
            super.finalize();
            if (USE_FINALIZER) {
                if (false && _ddnode_ptr >= 0) {
                    System.out.println("BDD not freed! "+System.identityHashCode(this));
                }
                this.free();
            }
        }
        */
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#free()
         */
        public void free() {
            delRef(_ddnode_ptr);
            _ddnode_ptr = INVALID_BDD;
        }
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#veccompose(org.sf.javabdd.BDDPairing)
         */
        public BDD veccompose(BDDPairing pair) {
            CALBDDPairing p = (CALBDDPairing) pair;
            long b = veccompose0(_ddnode_ptr, p._ptr);
            return new CALBDD(b);
        }
        private static native long veccompose0(long b, long p);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#replace(org.sf.javabdd.BDDPairing)
         */
        public BDD replace(BDDPairing pair) {
            CALBDDPairing p = (CALBDDPairing) pair;
            long b = replace0(_ddnode_ptr, p._ptr);
            return new CALBDD(b);
        }
        private static native long replace0(long b, long p);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#replaceWith(org.sf.javabdd.BDDPairing)
         */
        public BDD replaceWith(BDDPairing pair) {
            CALBDDPairing p = (CALBDDPairing) pair;
            long b = replace0(_ddnode_ptr, p._ptr);
            addRef(b);
            delRef(_ddnode_ptr);
            _ddnode_ptr = b;
            return this;
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#equals(org.sf.javabdd.BDD)
         */
        public boolean equals(BDD that) {
            return this._ddnode_ptr == ((CALBDD) that)._ddnode_ptr;
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDD#hashCode()
         */
        public int hashCode() {
            return (int) this._ddnode_ptr;
        }

    }
    
    /* (non-Javadoc)
     * An implementation of a BDDDomain, used by the CAL interface.
     */
    private static class CALBDDDomain extends BDDDomain {

        private CALBDDDomain(int index, long range) {
            super(index, range);
        }

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDDDomain#getFactory()
         */
        public BDDFactory getFactory() {
            return INSTANCE;
        }
        
    }

    /* (non-Javadoc)
     * An implementation of a BDDPairing, used by the CAL interface.
     */
    private static class CALBDDPairing extends BDDPairing {

        long _ptr;

        private CALBDDPairing() {
            _ptr = alloc();
        }

        private static native long alloc();

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDDPairing#set(int, int)
         */
        public void set(int oldvar, int newvar) {
            set0(_ptr, oldvar, newvar);
        }
        private static native void set0(long p, int oldvar, int newvar);

        /* (non-Javadoc)
         * @see org.sf.javabdd.BDDPairing#set(int, org.sf.javabdd.BDD)
         */
        public void set(int oldvar, BDD newvar) {
            CALBDD c = (CALBDD) newvar;
            set2(_ptr, oldvar, c._ddnode_ptr);
        }
        private static native void set2(long p, int oldvar, long newbdd);
        
        /* (non-Javadoc)
         * @see org.sf.javabdd.BDDPairing#reset()
         */
        public void reset() {
            reset0(_ptr);
        }
        private static native void reset0(long ptr);
        
        /**
         * Free the memory allocated for this pair.
         */
        private static native void free0(long ptr);
    }

    /* (non-Javadoc)
     * @see org.sf.javabdd.BDDFactory#createBitVector(int)
     */
    protected BDDBitVector createBitVector(int a) {
        return new CALBDDBitVector(a);
    }
    
    /* (non-Javadoc)
     * An implementation of a BDDBitVector, used by the CAL interface.
     */
    private static class CALBDDBitVector extends BDDBitVector {

        private CALBDDBitVector(int a) {
            super(a);
        }

        public BDDFactory getFactory() { return INSTANCE; }

    }
    
}
