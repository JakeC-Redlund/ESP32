"""Reject false confinement declarations while preserving task contracts."""
import copy
from pathlib import Path
import sys
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'scripts'))
from scaffold_audit_agent_process import agent_permission_contract_errors

class AgentPermissionContracts(unittest.TestCase):
    def test_all_owned_profiles_explain_inheritance(self):
        paths=sorted((ROOT/'.codex/agents').glob('*.toml'))
        self.assertEqual(35,len(paths))
        for p in paths:
            with self.subTest(profile=p.stem):
                self.assertEqual([],agent_permission_contract_errors(tomllib.loads(p.read_text()),p.name))

    def test_rejects_ignored_permission_keys_and_missing_role_contract(self):
        good={'name':'reviewer','developer_instructions':'Read-only task. Permissions inherit from the parent session. Do not edit.'}
        for key,value in [('sandbox_mode','read-only'),('sandbox_mode','workspace-write'),('approval_policy','never')]:
            with self.subTest(key=key,value=value):
                self.assertTrue(agent_permission_contract_errors({**good,key:value},'fixture'))
        self.assertTrue(agent_permission_contract_errors({**good,'developer_instructions':'Read-only only'},'fixture'))
        self.assertTrue(agent_permission_contract_errors({**good,'developer_instructions':'Permissions inherit from the parent session'},'fixture'))

if __name__=='__main__':unittest.main()
