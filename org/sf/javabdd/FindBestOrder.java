// FindBestOrder.java, created Apr 2, 2004 10:43:21 PM 2004 by jwhaley
// Copyright (C) 2004 John Whaley <jwhaley@alum.mit.edu>
// Licensed under the terms of the GNU LGPL; see COPYING for details.
package org.sf.javabdd;

import java.util.StringTokenizer;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigInteger;

/**
 * FindBestOrder
 * 
 * @author jwhaley
 * @version $Id: FindBestOrder.java,v 1.3 2004/04/04 18:34:14 joewhaley Exp $
 */
public class FindBestOrder {

    String filename0 = "fbo.bi";
    String filename1 = "fbo.1";
    String filename2 = "fbo.2";
    String filename3 = "fbo.3";
    
    long DELAY_TIME = 30000;
    
    BDDFactory.BDDOp op;
    long bestCalcTime;
    long bestTotalTime;
    String bestOrder;
    
    int nodeTableSize;
    int cacheSize;
    int maxIncrease;

    public FindBestOrder(BDD b1, BDD b2, BDD dom, BDDFactory.BDDOp op,
                         int cacheSize, int maxIncrease, long bestTime, long delayTime)
        throws IOException {
        this.op = op;
        this.bestCalcTime = bestTime;
        this.bestTotalTime = Long.MAX_VALUE;
        this.nodeTableSize = b1.getFactory().getAllocNum();
        this.cacheSize = cacheSize;
        this.maxIncrease = maxIncrease;
        this.DELAY_TIME = delayTime;
        File f = File.createTempFile("fbo", "a");
        filename0 = f.getAbsolutePath();
        f.deleteOnExit();
        f = File.createTempFile("fbo", "b");
        filename1 = f.getAbsolutePath();
        f.deleteOnExit();
        f = File.createTempFile("fbo", "c");
        filename2 = f.getAbsolutePath();
        f.deleteOnExit();
        f = File.createTempFile("fbo", "d");
        filename3 = f.getAbsolutePath();
        f.deleteOnExit();
        writeBDDConfig(b1.getFactory(), filename0);
        b1.getFactory().save(filename1, b1);
        b2.getFactory().save(filename2, b2);
        dom.getFactory().save(filename3, dom);
    }
    
    public void writeBDDConfig(BDDFactory bdd, String fileName) throws IOException {
        DataOutputStream dos = new DataOutputStream(new FileOutputStream(fileName));
        for (int i = 0; i < bdd.numberOfDomains(); ++i) {
            BDDDomain d = bdd.getDomain(i);
            dos.writeBytes(d.getName()+" "+d.size()+"\n");
        }
        dos.close();
    }
    
    public long tryOrder(boolean reverse, String varOrder) {
        System.gc();
        TryThread t = new TryThread();
        t.reverse = reverse;
        t.varOrderToTry = varOrder;
        t.start();
        try {
            long waitTime = bestTotalTime + DELAY_TIME;
            if (waitTime < 0L) waitTime = Long.MAX_VALUE;
            t.join(waitTime);
        } catch (InterruptedException x) {
        }
        t.stop();
        if (t.time < bestCalcTime) {
            bestOrder = varOrder;
            bestCalcTime = t.time;
            if (t.totalTime < bestTotalTime)
                bestTotalTime = t.totalTime;
        }
        return t.time;
    }
    
    public String getBestOrder() {
        return bestOrder;
    }
    
    public long getBestTime() {
        return bestCalcTime;
    }
    
    public class TryThread extends Thread {
        boolean reverse;
        String varOrderToTry;
        long time = Long.MAX_VALUE;
        long totalTime = Long.MAX_VALUE;
        
        public void run() {
            long total = System.currentTimeMillis();
            BDDFactory bdd = JavaFactory.init(nodeTableSize, cacheSize);
            bdd.setMaxIncrease(maxIncrease);
            readBDDConfig(bdd);
            int[] varorder = bdd.makeVarOrdering(reverse, varOrderToTry);
            bdd.setVarOrder(varorder);
            //System.out.println("\nTrying ordering "+varOrderToTry);
            try {
                BDD b1 = bdd.load(filename1);
                BDD b2 = bdd.load(filename2);
                BDD b3 = bdd.load(filename3);
                long t = System.currentTimeMillis();
                BDD result = b1.applyEx(b2, op, b3);
                time = System.currentTimeMillis() - t;
                b1.free(); b2.free(); b3.free(); result.free();
            } catch (IOException x) {
            }
            System.out.println("Ordering: "+varOrderToTry+" time: "+time);
            bdd.done();
            totalTime = System.currentTimeMillis() - total;
        }
        
        public void readBDDConfig(BDDFactory bdd) {
            try {
                BufferedReader in = new BufferedReader(new FileReader(filename0));
                for (;;) {
                    String s = in.readLine();
                    if (s == null || s.equals("")) break;
                    StringTokenizer st = new StringTokenizer(s);
                    String name = st.nextToken();
                    long size = Long.parseLong(st.nextToken())-1;
                    makeDomain(bdd, name, BigInteger.valueOf(size).bitLength());
                }
                in.close();
            } catch (IOException x) {
            }
        }
        
        BDDDomain makeDomain(BDDFactory bdd, String name, int bits) {
            BDDDomain d = bdd.extDomain(new long[] { 1L << bits })[0];
            d.setName(name);
            return d;
        }
    }
}
